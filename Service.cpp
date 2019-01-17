//
// Created by Shinan on 2019/1/9.
//

#include "Service.h"
#include <sys/prctl.h>


Service::Service();
bool Service::setThreadName(const string name){
    if(prctl(PR_SET_NAME, name.c_str(), 0, 0, 0) == -1 )
	{
		LOG(ERROR) << m_streamInfoKey << " set thread name failed! name:" << name;
		return false;
	}
	return true;
}