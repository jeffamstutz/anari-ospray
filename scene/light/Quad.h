// Copyright 2022 Intel Corporation
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "Light.h"

namespace anari_ospray {

struct QuadLight : public Light
{
  QuadLight(OSPRayGlobalState *d);
  void commit() override;
};

} // namespace anari_ospray
