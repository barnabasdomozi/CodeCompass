#ifndef CC_MODEL_CPPFILEMETRICS_H
#define CC_MODEL_CPPFILEMETRICS_H

#include <model/file.h>

namespace cc
{
namespace model
{

#pragma db object
struct CppFileMetrics
{
  enum Type
  {
    PLACEHOLDER
  };

  #pragma db id auto
  std::uint64_t id;

  #pragma db not_null
  FileId file;

  #pragma db not_null
  Type type;

  #pragma db not_null
  unsigned value;
};

} //model
} //cc

#endif //CC_MODEL_CPPFILEMETRICS_H
