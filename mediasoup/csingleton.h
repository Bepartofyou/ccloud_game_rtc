﻿/**
*
*	Copyright (C) 2010 FastTime
*  Description: 单例模式（线程安全）模板类
*	Author: syzsong
*	Date:	2016.4.20
*/
#ifndef _C_SINGLETON_H
#define _C_SINGLETON_H

namespace syz
{
	template < typename T >
	class csingleton
	{
	public:
		static T& get_instance()
		{
			static T v;
			return v;
		}
	};
}//namespace syz

#endif // !#define _C_SINGLETON_H
