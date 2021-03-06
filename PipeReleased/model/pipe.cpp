#include "pipe.h"
#include "../../PipeExport/exported_pipe.h"

pipe::pipe(const std::string &named_pipe)
{
  ep = new exported_pipe(named_pipe.c_str());
  ref = new store_type(this);
}

pipe::~pipe()
{
  delete ep;
}

object_interface pipe::Get(const OBJECT_TYPES type, const std::string &name)
{
  auto ObjectPostChange = [type](object_interface &obj)
  {
    auto record = ObjectsInfo().find(type);
    obj.object_info = record->second;
    return obj;
  };

  word id = GetID(type, name);
  return ObjectPostChange(Get(type, id));
}

object_interface pipe::Get(const OBJECT_TYPES type, const word id)
{
  throw_assert(id != 0);
  throw_assert(type != NOTANOBJECT);
  object_interface obj(*this, id, type);
  return obj;
}

object_interface pipe::Get(word _id)
{
  for (OBJECT_TYPES type : SupportedTypes())
  {
    auto t = Get(type, _id);
    if (t.GetID())
      return t;
  }
  throw_message("Object not found");
}

object_interface pipe::Get(const std::string &name)
{
  word res = 0;
  
  for (OBJECT_TYPES type : SupportedTypes())
  {
    auto t = Get(type, name);
    if (t.GetID())
      return t;
  }
  throw_message("Object not found");
}

word pipe::GetID(const OBJECT_TYPES type, const std::string &name)
{
  return ep->GetID(type, name.c_str());
}



#include "serialisation_route.h"

vector<param> pipe::GetRaw(const object_interface &obj)
{
  Update();
  auto server_object = ep->GetServerObject(obj.id);
  typedef std::tuple<objects::valve> supported_server_objects;
  
  try
  {
    auto ret = [&]()
    {
      serialisation_route<supported_server_objects> t;
      return t(server_object, obj.type);
    }();
    return ret;
  }
  catch (serialisation_failed)
  {
#if _DEBUG_
    throw_message("Server object didnt supported yet");
#endif
    return{};
  }
  dead_space();
}

REFACTOR // USE C++11
#include <time.h>

void pipe::Update()
{
  word now = clock();
  word second = CLOCKS_PER_SEC;
  word max_desync = second * 0.1;
  if (now - last_update <= max_desync)
    return;
  last_update = now;
  ep->Update();
}

void pipe::SetRaw(const object_interface &obj, vector<param> data)
{
  auto info = obj.GetInfo();
  for (auto p : data)
  {
    throw_sassert(p.id < info.params.size(), "Object parameter out of range");
    throw_sassert(!info.params[p.id].readonly, "Write protection on this parameter");
    auto ToDouble = [](const string &asn1)
    {
      return *reinterpret_cast<const double *>(asn1.c_str() + 2);
    };
    double 
      cur = ToDouble(p.raw.asn1_encoded_string), 
      prev = ToDouble(GetRaw(obj)[p.id].raw.asn1_encoded_string);
    string server_mapped_name = "�������"; // debug
    ep->SetServerTile(obj.GetID(), server_mapped_name.c_str(), original_source_code::enumValueDbl, &cur, &prev);
  }
  ep->Update();
  Update();
}

pipe::store_type pipe::Make()
{
  auto p = NEW MEMLEAK pipe();
  throw_assert(p->ref);
  return p->Ref();
}

void pipe::Shutdown()
{
  delete &Ref();
  shutdowned = true;
}

pipe::store_type &pipe::Ref() const
{
  throw_assert(!shutdowned);
  return *ref;
}
