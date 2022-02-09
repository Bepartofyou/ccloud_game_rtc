﻿#ifndef _C_TRANSPORT_H_
#define _C_TRANSPORT_H_
#include "mediasoupclient.hpp"
#include "cnet_types.h"
#include "json.hpp"
#include <memory>
#include "sdp/RemoteSdp.hpp"
#include "api/create_peerconnection_factory.h"
namespace chen {
	enum ERecv_Type
	{
		ERecv_None = 0,
		ERecv_Recving ,
		ERecv_Success
	};
	class cproducer;
	class cclient;
	class cdataconsumer;
	class ctransport : public webrtc::PeerConnectionObserver /*好玩东西给webrtc封装这个里面   */,
		public webrtc::CreateSessionDescriptionObserver
	{
		struct cDataConsmer
		{
			std::string m_id;
			std::string m_lable;
			std::string m_stream;
			std::string m_dataconsumerId;
			cDataConsmer() : m_id(""), m_stream(""), m_lable(""), m_dataconsumerId("")
			{}
		};
	public:
		ctransport( std::string transport_id, cclient *ptr);
		//ctransport() {}
		//~ctransport() {}
	public:

		bool init(bool send, const std::string &transport_id, const nlohmann::json& extendedRtpCapabilities,   const nlohmann::json& iceParameters,
			const nlohmann::json& iceCandidates,
			const nlohmann::json& dtlsParameters,
			const nlohmann::json& sctpParameters);
		


		// connect --> webrtc 
		bool webrtc_connect_transport_offer(webrtc::MediaStreamTrackInterface* track);
		bool webrtc_connect_transport_setup_connect(const std::string & localDtlsRole);
		// send connect localdes
		bool   webrtc_connect_transport_setup_connect_server_call();

		bool webrtc_transport_produce(const std::string & producerId);

		bool webrtc_connect_recv_setup_call();



		/// recv data conumser
		
		bool  webrtc_create_consumer(const std::string & id, const std::string & dataconsumerId, const std::string & label);
	public:
		 std::string  get_transportId() const { return m_transport_id; }
		 std::string get_kind() const { return m_track->kind(); }
		nlohmann::json get_sending_rtpParameters() const { return m_sendingRtpParametersByKind[m_track->kind()]; }
	public:

		//
		// PeerConnectionObserver implementation.
		//

		void OnSignalingChange(
			webrtc::PeerConnectionInterface::SignalingState new_state) override {}


		// 好家伙  webrtc封装太好 ^_^  接口定义 PeerConnectionObserver
		void OnAddTrack(
			rtc::scoped_refptr<webrtc::RtpReceiverInterface> receiver,
			const std::vector<rtc::scoped_refptr<webrtc::MediaStreamInterface>>&
			streams) override;
		void OnRemoveTrack(
			rtc::scoped_refptr<webrtc::RtpReceiverInterface> receiver) override;
		void OnDataChannel(
			rtc::scoped_refptr<webrtc::DataChannelInterface> channel) override {}
		void OnRenegotiationNeeded() override {}
		void OnIceConnectionChange(
			webrtc::PeerConnectionInterface::IceConnectionState new_state) override {}
		void OnIceGatheringChange(
			webrtc::PeerConnectionInterface::IceGatheringState new_state) override {}
		void OnIceCandidate(const webrtc::IceCandidateInterface* candidate) override;
		void OnIceConnectionReceivingChange(bool receiving) override {}
	public:
		// CreateSessionDescriptionObserver implementation.
		void OnSuccess(webrtc::SessionDescriptionInterface* desc) override;
		void OnFailure(webrtc::RTCError error) override;
	private:
		ctransport(const ctransport&);
		ctransport& operator =(const ctransport&);
	private:
		rtc::scoped_refptr<webrtc::PeerConnectionInterface> m_peer_connection;
		rtc::scoped_refptr<webrtc::PeerConnectionFactoryInterface> m_peer_connection_factory;
		std::unique_ptr<rtc::Thread> m_networkThread;
		std::unique_ptr<rtc::Thread> m_signalingThread;
		std::unique_ptr<rtc::Thread> m_workerThread;
		std::unique_ptr<mediasoupclient::Sdp::RemoteSdp>	 m_remote_sdp;
		std::string											 m_transport_id;
		rtc::scoped_refptr<webrtc::VideoTrackInterface> m_track;
		webrtc::RtpTransceiverInterface* m_transceiver;

		// TODO@chensong 20220207 
		nlohmann::json					m_sendingRtpParametersByKind;
		nlohmann::json					m_sendingRemoteRtpParametersByKind;
		

		std::unordered_map<std::string, std::shared_ptr<cproducer>> m_producers;

		cclient		*						m_client_ptr;
		std::string							m_offer;
		uint32								m_streamId;
		std::map<std::string, std::shared_ptr<cdataconsumer>> m_data_cosumsers;

		rtc::scoped_refptr<webrtc::DataChannelInterface> webrtcDataChannel;
		bool								m_send;
		ERecv_Type								m_recving;
		
		std::list<cDataConsmer>							m_dataconsmers;
		
	};
}
#endif // _C_TRANSPORT_H_