#pragma once
#define SunkueMakeVar(type, name) protected: type _##name
#define SunkueMakeSet(type, name) public: type& name(){return _##name;} 
#define SunkueMakeGet(type, name) public: const type& name()const { return _##name; } 
#define SunkueMakeGetSet(type, name) SunkueMakeGet(type, name) SunkueMakeSet(type, name) SunkueMakeVar(type, name)
