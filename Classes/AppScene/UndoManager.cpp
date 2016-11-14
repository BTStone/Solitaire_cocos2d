#include "UndoManager.h"

UndoManager* UndoManager::m_shared = NULL;

UndoManager* UndoManager::share()
{
	if (!m_shared)
	{
		m_shared = new UndoManager();
	}
	return m_shared;
}

void UndoManager::pushBack(UndoInfo info)
{
	if (!isInvalidUndoInfo(info))
	{
		m_infos.push_back(info);
	}
}

UndoInfo UndoManager::pop()
{
	int sz = m_infos.size();
	if (sz <= 0)
	{
		return Invalid_Undo_Info;
	}
	UndoInfo re = m_infos.back();
	m_infos.pop_back();
	return re;
}

bool UndoManager::isInvalidUndoInfo(UndoInfo info)
{
	return (info.m_number == 0);
}

void UndoManager::clear()
{
	m_infos.clear();
}