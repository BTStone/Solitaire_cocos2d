#ifndef __ST_REPORT_MANAGE_H__
#define __ST_REPORT_MANAGE_H__

#include "STBase.h"

class ReportManage
{
public:
	static ReportManage* share();

protected:
	static ReportManage* m_shared;
};


#endif
