#pragma once
#if !defined(RM_INSTANCEFILE_H_INC)
#define RM_INSTANCEFILE_H_INC

#ifdef DEBUG_LINKING
#pragma message("...including RM_InstanceFile.h")
#endif

class CRMInstance;

class CRMInstanceFile
{
public:
	CRMInstanceFile();
	~CRMInstanceFile();

	bool Open(const char *instance);
	void Close(void);
	CRMInstance *CreateInstance(const char *name);

protected:
	CGenericParser2 mParser;
	CGPGroup *mInstances;
};

#endif