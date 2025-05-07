#pragma once

#include <stdexcept>
#include <iostream>
#include <array>
#include <chrono>
#include <cstdlib>
#include <cstring>
#include <numeric>
#include <memory>
#include <string>
#include <vector>
#include <array>
#include <list>
#include <map>
#include <set>
#include <limits>
#include <stdexcept>
#include <algorithm>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

/*
#define DECLARE_SINGLETON(type)		\
private:							\
	type() {}						\
	~type() {}						\
public:								\
	static type* GetInstance()		\
	{								\
		static type instance;		\
		return &instance;			\
	}								\

#define GET_SINGLETON(type)			\
    type::GetInstance()			 
*/

#define DECLARE_SINGLETON(type)             \
private:                                    \
    static type* instance;                  \
    type() {}                               \
    ~type() {}                              \
public:                                     \
    static type* GetInstance()              \
    {                                       \
        if (!instance)                      \
            instance = new type();          \
        return instance;                    \
    }                                       \
    static void DestroyInstance()           \
    {                                       \
        if(instance)						\
		{									\
			delete instance;                \
        	instance = nullptr;             \
		}									\
    }

#define DEFINE_SINGLETON(type)              \
    type* type::instance = nullptr;

#define GET_SINGLETON(type)                 \
    type::GetInstance()

#define DESTROY_SINGLETON(type)             \
    type::DestroyInstance()

// End of singleton macros (Added to get rid of the 'backslash-newline at the end of file' warnings)