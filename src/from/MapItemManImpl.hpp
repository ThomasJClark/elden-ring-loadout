#pragma once

#include <fd4/component.hpp>
#include <fd4/detail/singleton.hpp>

namespace from
{
namespace CS
{

class MapItemManImpl : public FD4::FD4ComponentBase
{
  public:
    FD4_SINGLETON_CLASS(MapItemManImpl);

    virtual ~MapItemManImpl() = 0;
};

}
}
