// Copyright 2022 Intel Corporation
// SPDX-License-Identifier: Apache-2.0

#include "AO.h"

namespace anari_ospray {

AO::AO(OSPRayGlobalState *s) : Renderer(s, "ao") {}

void AO::commitParameters()
{
  Renderer::commitParameters();
  m_aoSamples = getParam<int>("aoSamples", 1);
  m_aoDistance = getParam<float>("aoDistance", 1e20f);
  m_aoIntensity = getParam<float>("aoIntensity", 1.f);
  m_volumeSamplingRate = getParam<float>("volumeSamplingRate", 1.f);
}

void AO::finalize()
{
  Renderer::finalize();

  auto r = osprayRenderer();
  ospSetInt(r, "aoSamples", m_aoSamples);
  ospSetFloat(r, "aoDistance", m_aoDistance);
  ospSetFloat(r, "aoIntensity", m_aoIntensity);
  ospSetFloat(r, "volumeSamplingRate", m_volumeSamplingRate);
  ospCommit(r);
}

} // namespace anari_ospray
