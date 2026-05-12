// Copyright 2022 Intel Corporation
// SPDX-License-Identifier: Apache-2.0

#include "TransferFunction1D.h"

namespace anari_ospray {
namespace {

bool hasCombinedColorAndOpacity(const Array1D &colorData)
{
  return colorData.elementType() == ANARI_FLOAT32_VEC4;
}

bool hasSeparateColor(const Array1D &colorData)
{
  return colorData.elementType() == ANARI_FLOAT32_VEC3;
}

bool hasSeparateOpacity(const Array1D &opacityData)
{
  return opacityData.elementType() == ANARI_FLOAT32;
}

void setSharedDataParam(OSPObject object,
    const char *name,
    const void *data,
    OSPDataType type,
    size_t size)
{
  auto d = ospNewSharedData1D(data, type, size);
  ospSetParam(object, name, OSP_DATA, &d);
  ospRelease(d);
}

} // namespace

TransferFunction1DVolume::TransferFunction1DVolume(OSPRayGlobalState *d)
    : Volume(d), m_colorData(this), m_opacityData(this)
{
  m_osprayTF = ospNewTransferFunction("piecewiseLinear");
}

TransferFunction1DVolume::~TransferFunction1DVolume()
{
  ospRelease(m_osprayTF);
}

void TransferFunction1DVolume::commitParameters()
{
  m_field = getParamObject<SpatialField>("value");
  m_colorData = getParamObject<Array1D>("color");
  m_opacityData = getParamObject<Array1D>("opacity");
  if (!getParam("valueRange", ANARI_FLOAT32_BOX1, &m_valueRange))
    m_valueRange = float2(0.f, 1.f);
  m_densityScale = getParam<float>("densityScale", 1.f);
}

void TransferFunction1DVolume::finalize()
{
  if (!m_field) {
    reportMessage(ANARI_SEVERITY_WARNING,
        "no spatial field provided to transfer function");
    return;
  }

  if (!m_colorData) {
    reportMessage(
        ANARI_SEVERITY_WARNING, "no color data provided to transfer function");
    return;
  }

  const auto colorIncludesOpacity = hasCombinedColorAndOpacity(*m_colorData);

  if (!colorIncludesOpacity && !m_opacityData) {
    reportMessage(ANARI_SEVERITY_WARNING,
        "no opacity data provided to transfer function");
    return;
  }

  if (!colorIncludesOpacity && !hasSeparateColor(*m_colorData)) {
    reportMessage(ANARI_SEVERITY_WARNING,
        "transfer function color data must be float3 when opacity is separate");
    return;
  }

  if (m_opacityData && !hasSeparateOpacity(*m_opacityData)) {
    reportMessage(
        ANARI_SEVERITY_WARNING, "transfer function opacity data must be float");
    return;
  }

  auto tf = m_osprayTF;
  ospSetParam(tf, "value", OSP_BOX1F, &m_valueRange);

  m_unpackedColors.clear();
  m_unpackedOpacities.clear();

  if (colorIncludesOpacity) {
    const auto size = m_colorData->size();
    const auto *colorAndOpacity = m_colorData->beginAs<float4>();
    m_unpackedColors.reserve(size);

    if (!m_opacityData)
      m_unpackedOpacities.reserve(size);

    for (size_t i = 0; i < size; ++i) {
      const auto &v = colorAndOpacity[i];
      m_unpackedColors.emplace_back(v.x, v.y, v.z);
      if (!m_opacityData)
        m_unpackedOpacities.emplace_back(v.w);
    }

    setSharedDataParam(tf,
        "color",
        m_unpackedColors.data(),
        OSP_VEC3F,
        m_unpackedColors.size());

    if (!m_opacityData) {
      setSharedDataParam(tf,
          "opacity",
          m_unpackedOpacities.data(),
          OSP_FLOAT,
          m_unpackedOpacities.size());
    }
  } else {
    setSharedDataParam(
        tf, "color", m_colorData->begin(), OSP_VEC3F, m_colorData->size());
  }

  if (m_opacityData) {
    setSharedDataParam(tf,
        "opacity",
        m_opacityData->begin(),
        OSP_FLOAT,
        m_opacityData->size());
  }

  ospCommit(tf);

  auto om = osprayModel();
  auto ov = m_field->osprayVolume();
  ospSetParam(om, "volume", OSP_VOLUME, &ov);
  ospSetParam(om, "densityScale", OSP_FLOAT, &m_densityScale);
  ospSetParam(om, "transferFunction", OSP_TRANSFER_FUNCTION, &tf);
  ospCommit(om);
}

bool TransferFunction1DVolume::isValid() const
{
  if (!m_field || !m_field->isValid() || !m_colorData)
    return false;

  if (m_opacityData && !hasSeparateOpacity(*m_opacityData))
    return false;

  return hasCombinedColorAndOpacity(*m_colorData)
      || (hasSeparateColor(*m_colorData) && m_opacityData);
}

} // namespace anari_ospray
