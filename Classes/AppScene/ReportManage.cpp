#include "ReportManage.h"

ReportManage* ReportManage::m_shared = NULL;

ReportManage* ReportManage::share()
{
	if (!m_shared)
	{
		m_shared = new ReportManage();
	}

	return m_shared;
}