#include "hzpch.h"
#include "OpenGLBuffer.h"
#include <Glad/glad.h>
namespace Hazel {
	//////////////////////////////////////////////////////////////////////////////////
	// VertexBuffer
	//////////////////////////////////////////////////////////////////////////////////
	OpenGLVertexBuffer::OpenGLVertexBuffer(unsigned int size)
		: m_RendererID(0), m_Size(size)
	{
		HZ_RENDER_S({
			glGenBuffers(1, &self->m_RendererID);
			});
	}
	OpenGLVertexBuffer::~OpenGLVertexBuffer()
	{
		HZ_RENDER_S({
			glDeleteBuffers(1, &self->m_RendererID);
			});
	}
	void OpenGLVertexBuffer::SetData(void* buffer, unsigned int size, unsigned int offset)
	{
		HZ_RENDER_S3(buffer, size, offset, {
			glBindBuffer(GL_ARRAY_BUFFER, self->m_RendererID);
			glBufferData(GL_ARRAY_BUFFER, size, buffer, GL_STATIC_DRAW);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);
			});
	}
	void OpenGLVertexBuffer::Bind() const
	{
		HZ_RENDER_S({
			glBindBuffer(GL_ARRAY_BUFFER, self->m_RendererID);
			});
	}
	//////////////////////////////////////////////////////////////////////////////////
	// IndexBuffer
	//////////////////////////////////////////////////////////////////////////////////
	OpenGLIndexBuffer::OpenGLIndexBuffer(unsigned int size)
		: m_RendererID(0), m_Size(size)
	{
		HZ_RENDER_S({
			glGenBuffers(1, &self->m_RendererID);
			});
	}
	OpenGLIndexBuffer::~OpenGLIndexBuffer()
	{
		HZ_RENDER_S({
			glDeleteBuffers(1, &self->m_RendererID);
			});
	}
	void OpenGLIndexBuffer::SetData(void* buffer, unsigned int size, unsigned int offset)
	{
		HZ_RENDER_S3(buffer, size, offset, {
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, self->m_RendererID);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, buffer, GL_STATIC_DRAW);
			});
	}
	void OpenGLIndexBuffer::Bind() const
	{
		HZ_RENDER_S({
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, self->m_RendererID);
			});
	}
}