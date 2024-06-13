// automatically generated by the FlatBuffers compiler, do not modify


#ifndef FLATBUFFERS_GENERATED_SCANHEADTYPE_JOESCAN_SCHEMA_H_
#define FLATBUFFERS_GENERATED_SCANHEADTYPE_JOESCAN_SCHEMA_H_

#include "flatbuffers/flatbuffers.h"

namespace joescan {
namespace schema {

enum ScanHeadType : uint16_t {
  ScanHeadType_INVALID = 0,
  ScanHeadType_JS50WX = 1,
  ScanHeadType_JS50WSC = 2,
  ScanHeadType_JS50X6B20 = 3,
  ScanHeadType_JS50X6B30 = 4,
  ScanHeadType_JS50MX = 5,
  ScanHeadType_JS50Z8 = 6,
  ScanHeadType_MIN = ScanHeadType_INVALID,
  ScanHeadType_MAX = ScanHeadType_JS50Z8
};

inline const ScanHeadType (&EnumValuesScanHeadType())[7] {
  static const ScanHeadType values[] = {
    ScanHeadType_INVALID,
    ScanHeadType_JS50WX,
    ScanHeadType_JS50WSC,
    ScanHeadType_JS50X6B20,
    ScanHeadType_JS50X6B30,
    ScanHeadType_JS50MX,
    ScanHeadType_JS50Z8
  };
  return values;
}

inline const char * const *EnumNamesScanHeadType() {
  static const char * const names[8] = {
    "INVALID",
    "JS50WX",
    "JS50WSC",
    "JS50X6B20",
    "JS50X6B30",
    "JS50MX",
    "JS50Z8",
    nullptr
  };
  return names;
}

inline const char *EnumNameScanHeadType(ScanHeadType e) {
  if (flatbuffers::IsOutRange(e, ScanHeadType_INVALID, ScanHeadType_JS50Z8)) return "";
  const size_t index = static_cast<size_t>(e);
  return EnumNamesScanHeadType()[index];
}

}  // namespace schema
}  // namespace joescan

#endif  // FLATBUFFERS_GENERATED_SCANHEADTYPE_JOESCAN_SCHEMA_H_
