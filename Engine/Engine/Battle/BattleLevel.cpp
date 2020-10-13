#include "BattleLevel.hpp"

RTTR_REGISTRATION {
	registration::class_<BattleLevel>("BattleLevel")
		.public_object_constructor;
}

//static void rttr_auto_register_reflection_function_();                  
//namespace                                                               
//{                                                                       
//    struct rttr__auto__register__                                       
//    {                                                                   
//        rttr__auto__register__()                                        
//        {                                                               
//            rttr_auto_register_reflection_function_();                  
//        }                                                               
//    };                                                                  
//}                                                                       
//static const rttr__auto__register__ RTTR_CAT(auto_register__,__LINE__); 
//static void rttr_auto_register_reflection_function_() {
//    registration::class_<BattleLevel>("BattleLevel")
//        .public_object_constructor;
//}

void BattleLevel::Init() { }

void BattleLevel::Update() { }

void BattleLevel::LateUpdate() { }

void BattleLevel::Exit() { }
