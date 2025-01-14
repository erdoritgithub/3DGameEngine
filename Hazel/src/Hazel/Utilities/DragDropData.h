#pragma once
namespace Hazel {
	struct DragDropData
	{
	public:
		const char* Type;
		const char* SourcePath;
		const char* Name;
		DragDropData(const char* type, const char* sourcePath, const char* name)
			: Type(type), SourcePath(sourcePath), Name(name)
		{
		}
	};
}