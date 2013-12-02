namespace original_project_source_code
{
  // BaseType.h : 43
  typedef enum  EDataTypes
  {
    // ��������� ������
#undef  DATATYPE
#define DATATYPE( Id, Nomer, Text, Icon ) id_##Id = Nomer,
#include <BaseType.hpp>
    //
    id_ObjInfo = 19,// 19 - ����������� ���������� �� �������
    id_GetInfo = 115,// 115
    id_PHS = 120,                     // 120 ���������������� ����
    id_Intermodel = 121,// 121 ������������ �����
    // ��������� Yokogawa
    id_AcyMin = 0x0200,
#undef   YOKO_TYPE
#define  YOKO_TYPE( a, b, c ) id_##b = 0x##a,
#include <YokogawaType.hpp>
    id_AcyMax = 0x530C,
  } EDataTypes;
#undef DATA_TYPE
#undef YOKO_TYPE
}

enum OBJECT_TYPES
{
  VALVE = original_project_source_code::id_Klapan,
  GATE_VALVE = original_project_source_code::id_Zadv,
  KVO = original_project_source_code::id_KVO,
  HS = original_project_source_code::id_HS,
  PUMP = original_project_source_code::id_Pump
};