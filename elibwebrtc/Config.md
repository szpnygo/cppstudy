// 下面的字段与标准的RTCConfiguration字典相对应：
// https://w3c.github.io/webrtc-pc/#rtcconfiguration-dictionary

IceServers servers;  // Ice服务器列表
IceTransportsType type = kAll;  // Ice传输类型，默认为全部
BundlePolicy bundle_policy = kBundlePolicyBalanced;  // Bundle策略，默认为平衡
RtcpMuxPolicy rtcp_mux_policy = kRtcpMuxPolicyRequire;  // Rtcp复用策略，默认为必需
std::vector<rtc::scoped_refptr<rtc::RTCCertificate>> certificates;  // 证书列表
int ice_candidate_pool_size = 0;  // Ice候选池大小，默认为0


class RTCMediaConstraints : public RefCountInterface {
 public:
  // These keys are google specific.
  // Google特有的回声消除开关
  LIB_WEBRTC_API static const char* kGoogEchoCancellation;  

  // Google特有的扩展滤波器回声消除开关
  LIB_WEBRTC_API static const char* kExtendedFilterEchoCancellation;  

  // Google特有的DA回声消除开关
  LIB_WEBRTC_API static const char* kDAEchoCancellation;     

  // Google特有的自动增益控制开关
  LIB_WEBRTC_API static const char* kAutoGainControl;  

  // Google特有的噪声抑制开关
  LIB_WEBRTC_API static const char* kNoiseSuppression;  

  // Google特有的高通滤波器开关
  LIB_WEBRTC_API static const char* kHighpassFilter;    

  // Google特有的音频镜像开关
  LIB_WEBRTC_API static const char* kAudioMirroring;    

  // Google特有的音频网络适配器配置开关
  LIB_WEBRTC_API static const char* kAudioNetworkAdaptorConfig;  

  // CreateOffer / CreateAnswer的约束键，由W3C PeerConnection规范规定
  // 是否提供接收视频流的功能
  LIB_WEBRTC_API static const char* kOfferToReceiveVideo;  

  // 是否提供接收音频流的功能
  LIB_WEBRTC_API static const char* kOfferToReceiveAudio;  

  // 语音活动检测开关
  LIB_WEBRTC_API static const char* kVoiceActivityDetection;  

  // ICE重启开关
  LIB_WEBRTC_API static const char* kIceRestart;  

  // Google特有的使用RTP Mux的开关
  LIB_WEBRTC_API static const char* kUseRtpMux;  

  // 约束的值，"true"或"false"
  LIB_WEBRTC_API static const char* kValueTrue;   
  LIB_WEBRTC_API static const char* kValueFalse;  

  // 用于启用DataChannels的临时伪约束
  LIB_WEBRTC_API static const char* kEnableRtpDataChannels;  

  // 用于通过JS启用DSCP的临时伪约束
  LIB_WEBRTC_API static const char* kEnableDscp;  

  // 通过JS启用IPv6的约束
  LIB_WEBRTC_API static const char* kEnableIPv6;  

  // 用于启用低于最小比特率暂停功能的临时约束
  LIB_WEBRTC_API static const char* kEnableVideoSuspendBelowMinBitrate;

  // 用于启用联合音视频带宽估计的约束
  LIB_WEBRTC_API static const char* kCombinedAudioVideoBwe;  

  // 用于启用屏幕广播最小比特率的约束
  LIB_WEBRTC_API static const char* kScreencastMinBitrate;  

  // 用于启用CPU过载检测的约束
  LIB_WEBRTC_API static const char* kCpuOveruseDetection;  

  // 指定对所有视频轨道的并发层的数量
  // (见RTCOfferAnswerOptions::num_simulcast_layers).
  LIB_WEBRTC_API static const char* kNumSimulcastLayers;

 public:
  // 创建RTCMediaConstraints对象的工厂方法
  LIB_WEBRTC_API static scoped_refptr<RTCMediaConstraints> Create();

  // 添加必要的约束
  virtual void AddMandatoryConstraint(const string key, const string value) = 0;

  // 添加可选的约束
  virtual void AddOptionalConstraint(const string key, const string value) = 0;

 protected:
  // 虚析构函数，用于支持正确的资源释放
  virtual ~RTCMediaConstraints() {}
};
