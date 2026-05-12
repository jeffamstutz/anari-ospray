// Copyright 2022 Intel Corporation
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "Renderer.h"

namespace anari_ospray {

struct AO : public Renderer
{
  AO(OSPRayGlobalState *s);
  void commitParameters() override;
  void finalize() override;

 private:
  int m_aoSamples{1};
  float m_aoDistance{1e20f};
  float m_aoIntensity{1.f};
  float m_volumeSamplingRate{1.f};
};

} // namespace anari_ospray
