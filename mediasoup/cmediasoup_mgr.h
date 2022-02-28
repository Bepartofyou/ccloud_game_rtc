/***********************************************************************************************
created: 		2019-03-02

author:			syzsong

purpose:		log
************************************************************************************************/
#ifndef C_MEDIASOUP_H
#define C_MEDIASOUP_H
#include <thread>
 
namespace cmediasoup
{
	class __declspec(dllimport) cmediasoup_mgr
	{
	public:
		cmediasoup_mgr();
		~cmediasoup_mgr();
	public:
		bool init(const char * file_name);

		void startup();

		void destroy();

		void   webrtc_video(unsigned char * rgba_ptr, int width, int height);

		void webrtc_pause();
		void webrtc_resume();
		bool webrtc_video_staus() const { return m_webrtc_pause; }
		bool  mediasoup_run();
		
	private:
		void _mediasoup_thread();
	private:
		cmediasoup_mgr(const cmediasoup_mgr&);
		cmediasoup_mgr& operator =(const cmediasoup_mgr&);
	private:
		bool				m_init ;
		std::thread			m_thread;
		bool				m_webrtc_pause;
	};
}
#endif // C_MEDIASOUP_H