#pragma once

#include "../mapper/mapper.h"

#include "exported\air_condenser.h"
#include "exported\gate_valve.h"
#include "exported\hs.h"
#include "exported\pump.h"
#include "exported\scheme_data.h"
#include "exported\sensor.h"
#include "exported\valve.h"

#include "object_types.h"
#include <vector>
const std::vector<OBJECT_TYPES> &SupportedTypes();

#include "../../interface/info.h"
#include <map>
const std::map<OBJECT_TYPES, info> &ObjectsInfo();