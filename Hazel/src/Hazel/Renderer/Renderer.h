#pragma once

#include "RenderCommandQueue.h"
#include "RendererAPI.h"

namespace Hazel {

	class Renderer
	{
	public:
		typedef void(*RenderCommandFn)(void*);

		// Commands
		static void Clear();
		static void Clear(float r, float g, float b, float a = 1.0f);
		static void SetClearColor(float r, float g, float b, float a);

		static void ClearMagenta();

		void Init();

		static void* Submit(RenderCommandFn fn, unsigned int size)
		{
			return s_Instance->m_CommandQueue.Allocate(fn, size);
		}

		void WaitAndRender();

		inline static Renderer& Get() { return *s_Instance; }
	private:
		static Renderer* s_Instance;

		RenderCommandQueue m_CommandQueue;
	};

}

#define HZ_RENDER_PASTE2(a, b) a ## b
#define HZ_RENDER_PASTE(a, b) HZ_RENDER_PASTE2(a, b)
#define HZ_RENDER_UNIQUE(x) HZ_RENDER_PASTE(x, __LINE__)

#define HZ_RENDER(code) \
    struct HZ_RENDER_UNIQUE(HZRenderCommand) \
    {\
        static void Execute(void*)\
        {\
            code\
        }\
    };\
	{\
		auto mem = RenderCommandQueue::Submit(sizeof(HZ_RENDER_UNIQUE(HZRenderCommand)), HZ_RENDER_UNIQUE(HZRenderCommand)::Execute);\
		new (mem) HZ_RENDER_UNIQUE(HZRenderCommand)();\
	}\

#define HZ_RENDER_1(arg0, code) \
	do {\
    struct HZ_RENDER_UNIQUE(HZRenderCommand) \
    {\
		HZ_RENDER_UNIQUE(HZRenderCommand)(typename ::std::remove_const<typename ::std::remove_reference<decltype(arg0)>::type>::type arg0) \
		: arg0(arg0) {}\
		\
        static void Execute(void* self)\
        {\
			auto& arg0 = ((HZ_RENDER_UNIQUE(HZRenderCommand)*)self)->arg0;\
            code\
        }\
		\
		typename ::std::remove_const<typename ::std::remove_reference<decltype(arg0)>::type>::type arg0;\
    };\
	{\
		auto mem = ::Hazel::Renderer::Submit(HZ_RENDER_UNIQUE(HZRenderCommand)::Execute, sizeof(HZ_RENDER_UNIQUE(HZRenderCommand)));\
		new (mem) HZ_RENDER_UNIQUE(HZRenderCommand)(arg0);\
	} } while(0)

#define HZ_RENDER_2(arg0, arg1, code) \
    struct HZ_RENDER_UNIQUE(HZRenderCommand) \
    {\
		HZ_RENDER_UNIQUE(HZRenderCommand)(typename ::std::remove_const<typename ::std::remove_reference<decltype(arg0)>::type>::type arg0,\
											typename ::std::remove_const<typename ::std::remove_reference<decltype(arg1)>::type>::type arg1) \
		: arg0(arg0), arg1(arg1) {}\
		\
        static void Execute(void* self)\
        {\
			auto& arg0 = ((HZ_RENDER_UNIQUE(HZRenderCommand)*)self)->arg0;\
			auto& arg1 = ((HZ_RENDER_UNIQUE(HZRenderCommand)*)self)->arg1;\
            code\
        }\
		\
		typename ::std::remove_const<typename ::std::remove_reference<decltype(arg0)>::type>::type arg0;\
		typename ::std::remove_const<typename ::std::remove_reference<decltype(arg1)>::type>::type arg1;\
    };\
	{\
		auto mem = ::Hazel::Renderer::Submit(HZ_RENDER_UNIQUE(HZRenderCommand)::Execute, sizeof(HZ_RENDER_UNIQUE(HZRenderCommand)));\
		new (mem) HZ_RENDER_UNIQUE(HZRenderCommand)(arg0, arg1);\
	}\

#define HZ_RENDER_3(arg0, arg1, arg2, code) \
    struct HZ_RENDER_UNIQUE(HZRenderCommand) \
    {\
		HZ_RENDER_UNIQUE(HZRenderCommand)(typename ::std::remove_const<typename ::std::remove_reference<decltype(arg0)>::type>::type arg0,\
											typename ::std::remove_const<typename ::std::remove_reference<decltype(arg1)>::type>::type arg1,\
											typename ::std::remove_const<typename ::std::remove_reference<decltype(arg2)>::type>::type arg2) \
		: arg0(arg0), arg1(arg1), arg2(arg2) {}\
		\
        static void Execute(void* self)\
        {\
			auto& arg0 = ((HZ_RENDER_UNIQUE(HZRenderCommand)*)self)->arg0;\
			auto& arg1 = ((HZ_RENDER_UNIQUE(HZRenderCommand)*)self)->arg1;\
			auto& arg2 = ((HZ_RENDER_UNIQUE(HZRenderCommand)*)self)->arg2;\
            code\
        }\
		\
		typename ::std::remove_const<typename ::std::remove_reference<decltype(arg0)>::type>::type arg0;\
		typename ::std::remove_const<typename ::std::remove_reference<decltype(arg1)>::type>::type arg1;\
		typename ::std::remove_const<typename ::std::remove_reference<decltype(arg2)>::type>::type arg2;\
    };\
	{\
		auto mem = ::Hazel::Renderer::Submit(HZ_RENDER_UNIQUE(HZRenderCommand)::Execute, sizeof(HZ_RENDER_UNIQUE(HZRenderCommand)));\
		new (mem) HZ_RENDER_UNIQUE(HZRenderCommand)(arg0, arg1, arg2);\
	}\

#define HZ_RENDER_4(arg0, arg1, arg2, arg3, code) \
    struct HZ_RENDER_UNIQUE(HZRenderCommand) \
    {\
		HZ_RENDER_UNIQUE(HZRenderCommand)(typename ::std::remove_const<typename ::std::remove_reference<decltype(arg0)>::type>::type arg0,\
											typename ::std::remove_const<typename ::std::remove_reference<decltype(arg1)>::type>::type arg1,\
											typename ::std::remove_const<typename ::std::remove_reference<decltype(arg2)>::type>::type arg2,\
											typename ::std::remove_const<typename ::std::remove_reference<decltype(arg3)>::type>::type arg3)\
		: arg0(arg0), arg1(arg1), arg2(arg2), arg3(arg3) {}\
		\
        static void Execute(void* self)\
        {\
			auto& arg0 = ((HZ_RENDER_UNIQUE(HZRenderCommand)*)self)->arg0;\
			auto& arg1 = ((HZ_RENDER_UNIQUE(HZRenderCommand)*)self)->arg1;\
			auto& arg2 = ((HZ_RENDER_UNIQUE(HZRenderCommand)*)self)->arg2;\
			auto& arg3 = ((HZ_RENDER_UNIQUE(HZRenderCommand)*)self)->arg3;\
            code\
        }\
		\
		typename ::std::remove_const<typename ::std::remove_reference<decltype(arg0)>::type>::type arg0;\
		typename ::std::remove_const<typename ::std::remove_reference<decltype(arg1)>::type>::type arg1;\
		typename ::std::remove_const<typename ::std::remove_reference<decltype(arg2)>::type>::type arg2;\
		typename ::std::remove_const<typename ::std::remove_reference<decltype(arg3)>::type>::type arg3;\
    };\
	{\
		auto mem = Renderer::Submit(HZ_RENDER_UNIQUE(HZRenderCommand)::Execute, sizeof(HZ_RENDER_UNIQUE(HZRenderCommand)));\
		new (mem) HZ_RENDER_UNIQUE(HZRenderCommand)(arg0, arg1, arg2, arg3);\
	}