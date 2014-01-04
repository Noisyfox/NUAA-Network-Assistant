#include "stdafx.h"
#include "Util.h"

DWORD SwapEndian(DWORD value)
{
	return ((value >> 24) & 0xFF) |
		((value >> 8) & 0xFF00) |
		((value << 8) & 0xFF0000) |
		((value << 24) & 0xFF000000);
}

BOOL NetAddr(CString s, DWORD & i){

	if (s == _T("255.255.255.255")){
		i = -1;
	}
	else {
		USES_CONVERSION;
		DWORD p = inet_addr(T2A(s));
		if (p == INADDR_NONE)return FALSE;
		i = p;
	}

	return TRUE;
}

CString DialGetError(DWORD errCode)
{
	CString str;

	switch (errCode){
	case 600:
		str = _T("操作挂起。");
		break;
	case 601:
		str = _T("检测到无效的端口句柄。");
		break;
	case 602:
		str = _T("指定的端口已打开。");
		break;
	case 603:
		str = _T("呼叫人的缓冲区太小。");
		break;
	case 604:
		str = _T("指定了不正确的信息。");
		break;
	case 605:
		str = _T("不能设置端口信息。");
		break;
	case 606:
		str = _T("指定的端口未连接。");
		break;
	case 607:
		str = _T("检测到无效事件。");
		break;
	case 608:
		str = _T("指定的设备不存在。");
		break;
	case 609:
		str = _T("指定的设备类型不存在。");
		break;
	case 610:
		str = _T("指定的缓冲区无效。");
		break;
	case 611:
		str = _T("指定的路由不可用。");
		break;
	case 612:
		str = _T("指定的路由未分配。");
		break;
	case 613:
		str = _T("指定的压缩无效。");
		break;
	case 614:
		str = _T("没有足够的缓冲区可用。");
		break;
	case 615:
		str = _T("未找到指定的端口。");
		break;
	case 616:
		str = _T("异步请求挂起。");
		break;
	case 617:
		str = _T("调制解调器已经断开连接。");
		break;
	case 618:
		str = _T("指定的端口未打开。");
		break;
	case 619:
		str = _T("指定的端口未连接。");
		break;
	case 620:
		str = _T("无法决定端点。");
		break;
	case 621:
		str = _T("系统无法打开电话簿。");
		break;
	case 622:
		str = _T("系统无法加载电话簿。");
		break;
	case 623:
		str = _T("系统无法找到此连接的电话簿项。");
		break;
	case 624:
		str = _T("系统无法更新电话簿文件。");
		break;
	case 625:
		str = _T("系统在电话簿中找到无效信息。");
		break;
	case 626:
		str = _T("无法加载字符串。");
		break;
	case 627:
		str = _T("无法找到关键字。");
		break;
	case 628:
		str = _T("连接被关闭。");
		break;
	case 629:
		str = _T("连接被远程计算机关闭。");
		break;
	case 630:
		str = _T("由于硬件故障，调制解调器断开连接。");
		break;
	case 631:
		str = _T("用户断开了调制解调器连接。");
		break;
	case 632:
		str = _T("检测到不正确的结构大小。");
		break;
	case 633:
		str = _T("调制解调器正在使用或没有配置为拨出。");
		break;
	case 634:
		str = _T("您的计算机无法在远程网络上注册。");
		break;
	case 635:
		str = _T("出现未知错误。");
		break;
	case 636:
		str = _T("连接到端口的设备不是所期望的设备。");
		break;
	case 637:
		str = _T("检测到不能转换的字符串。");
		break;
	case 638:
		str = _T("请求超时。");
		break;
	case 639:
		str = _T("异步网络不可用。");
		break;
	case 640:
		str = _T("发生与 NetBIOS 有关的错误。");
		break;
	case 641:
		str = _T("服务器不能分配支持客户机所需的 NetBIOS 资源。");
		break;
	case 642:
		str = _T("计算机的某个 NetBIOS 名已经在远程网络上注册。");
		break;
	case 643:
		str = _T("服务器端的网卡出现故障。");
		break;
	case 644:
		str = _T("您将无法接收网络弹出式消息。");
		break;
	case 645:
		str = _T("发生内部身份验证错误。");
		break;
	case 646:
		str = _T("此时间不允许该帐户登录。");
		break;
	case 647:
		str = _T("此帐户被禁用。");
		break;
	case 648:
		str = _T("该帐户的密码已过期。");
		break;
	case 649:
		str = _T("帐户没有拨入的权限。");
		break;
	case 650:
		str = _T("远程访问服务器没有响应。");
		break;
	case 651:
		str = _T("调制解调器报告错误。");
		break;
	case 652:
		str = _T("有一个来自调制解调器的无法识别的响应。");
		break;
	case 653:
		str = _T("在设备 .INF 文件部分中未找到调制解调器所请求的宏。");
		break;
	case 654:
		str = _T("设备 .INF 文件部分中的命令或响应引用了未定义的宏。");
		break;
	case 655:
		str = _T("在设备 .INF 文件部分中未找到 <MESSAGE> 宏。");
		break;
	case 656:
		str = _T("在设备 .INF 文件部分中的 <DEFAULTOFF> 宏包含未定义的宏。");
		break;
	case 657:
		str = _T("无法打开设备 .INF 文件。");
		break;
	case 658:
		str = _T("在设备 .INF 或媒体 .INI 文件中的设备名太长。");
		break;
	case 659:
		str = _T("媒体 .INI 文件引用了未知的设备名。");
		break;
	case 660:
		str = _T("设备 .INF 文件不包含对命令的响应。");
		break;
	case 661:
		str = _T("设备 .INF 文件缺少命令。");
		break;
	case 662:
		str = _T("试图设置设备 .INF 文件部分没有列出的宏。");
		break;
	case 663:
		str = _T("媒体 .INI 文件引用了未知的设备类型。");
		break;
	case 664:
		str = _T("系统内存不足。");
		break;
	case 665:
		str = _T("未正确配置调制解调器。");
		break;
	case 666:
		str = _T("调制解调器未正常工作。");
		break;
	case 667:
		str = _T("系统不能读取媒体 .INI 文件。");
		break;
	case 668:
		str = _T("连接被终止。");
		break;
	case 669:
		str = _T("媒体 .INI 文件中的用法参数无效。");
		break;
	case 670:
		str = _T("系统不能从媒体 .INI 文件中读取部分名称。");
		break;
	case 671:
		str = _T("系统不能从媒体 .INI 文件中读取设备类型。");
		break;
	case 672:
		str = _T("系统不能从媒体 .INI 文件中读取设备名称。");
		break;
	case 673:
		str = _T("系统不能从媒体 .INI 文件中读取用法。");
		break;
	case 674:
		str = _T("系统不能从媒体 .INI 文件中读取最大的连接速率 (bps)。");
		break;
	case 675:
		str = _T("系统不能从媒体 .INI 文件中读取最大的载波连接速度。");
		break;
	case 676:
		str = _T("电话线忙。");
		break;
	case 677:
		str = _T("是由人而不是调制解调器应答。");
		break;
	case 678:
		str = _T("没有应答。");
		break;
	case 679:
		str = _T("系统无法检测载波。");
		break;
	case 680:
		str = _T("没有拨号音。");
		break;
	case 681:
		str = _T("调制解调器报告一般错误。");
		break;
	case 691:
		str = _T("因为用户名和/或密码在此域上无效，所以访问被拒绝。");
		break;
	case 692:
		str = _T("调制解调器出现硬件故障。");
		break;
	case 695:
		str = _T("未启动状态机器。");
		break;
	case 696:
		str = _T("已启动状态机器。");
		break;
	case 697:
		str = _T("响应循环未完成。");
		break;
	case 699:
		str = _T("调制解调器的响应导致缓冲区溢出。");
		break;
	case 700:
		str = _T("设备 .INF 文件中的扩展命令太长。");
		break;
	case 701:
		str = _T("调制解调器使用了 COM 驱动程序不支持的连接速度。");
		break;
	case 703:
		str = _T("连接需要用户信息，但应用程序不允许用户交互。");
		break;
	case 704:
		str = _T("回拨号码无效。");
		break;
	case 705:
		str = _T("身份验证状态无效。");
		break;
	case 707:
		str = _T("出现与 X.25 协议有关的错误。");
		break;
	case 708:
		str = _T("帐户过期。");
		break;
	case 709:
		str = _T("更改域上的密码时发生错误。");
		break;
	case 710:
		str = _T("当与调制解调器通讯时检测到序列溢出错误。");
		break;
	case 711:
		str = _T("远程访问服务管理器无法启动事件日志中提供了其他信息。");
		break;
	case 712:
		str = _T("双路端口正在初始化。");
		break;
	case 713:
		str = _T("没有活动的 ISDN 线路可用。");
		break;
	case 714:
		str = _T("没有 ISDN 信道可用于拨号。");
		break;
	case 715:
		str = _T("由于电话线质量差，所以发生过多错误。");
		break;
	case 716:
		str = _T("远程访问服务 IP 配置不可用。");
		break;
	case 717:
		str = _T("在远程访问服务 IP 地址的静态池中没有 IP 地址可用。");
		break;
	case 718:
		str = _T("等待远程计算机有效响应的连接超时。");
		break;
	case 719:
		str = _T("连接被远程计算机终止。");
		break;
	case 720:
		str = _T("由于您的计算机与远程计算机的 PPP 控制协议不一致，所以连接尝试失败。");
		break;
	case 721:
		str = _T("远程计算机没有响应。");
		break;
	case 722:
		str = _T("从远程计算机接收到无效的数据。");
		break;
	case 723:
		str = _T("电话号码（包含前缀和后缀）太长。");
		break;
	case 726:
		str = _T("IPX 协议不能用于在多个的调制解调器上同时向外拨号。");
		break;
	case 728:
		str = _T("系统找不到 IP 适配器。");
		break;
	case 729:
		str = _T("除非安装 IP 协议，否则不能使用 SLIP。");
		break;
	case 731:
		str = _T("未配置协议。");
		break;
	case 732:
		str = _T("您的计算机和远程计算机的 PPP 控制协议无法一致。");
		break;
	case 733:
		str = _T("您的计算机和远程计算机的 PPP 控制协议无法一致。");
		break;
	case 734:
		str = _T("PPP 链接控制协议被终止。");
		break;
	case 735:
		str = _T("请求的地址被服务器拒绝。");
		break;
	case 736:
		str = _T("远程计算机终止了控制协议。");
		break;
	case 737:
		str = _T("检测到环回。");
		break;
	case 738:
		str = _T("服务器没有指派地址。");
		break;
	case 739:
		str = _T("远程服务器所需的身份验证协议不能使用存储的密码。");
		break;
	case 740:
		str = _T("检测到无效拨号规则。");
		break;
	case 741:
		str = _T("本地计算机不支持所需的数据加密类型。");
		break;
	case 742:
		str = _T("远程计算机不支持所需的数据加密类型。");
		break;
	case 743:
		str = _T("远程服务器要求数据加密。");
		break;
	case 751:
		str = _T("回拨号码包含无效的字符。");
		break;
	case 752:
		str = _T("当处理脚本时遇到语法错误。");
		break;
	case 753:
		str = _T("由于连接是由多协议路由器创建的，因此该连接无法断开。");
		break;
	case 754:
		str = _T("系统无法找到多链路绑定。");
		break;
	case 755:
		str = _T("由于该项已经指定自定义的拨号程序，因此系统不能执行自动拨号。");
		break;
	case 756:
		str = _T("已经拨打该连接。");
		break;
	case 757:
		str = _T("远程访问服务不能自动启动。事件日志中提供了其他信息。");
		break;
	case 758:
		str = _T("该连接上已经启用 Internet 连接共享。");
		break;
	case 760:
		str = _T("启用路由功能时发生错误。");
		break;
	case 761:
		str = _T("启用连接的 Internet 连接共享时发生错误。");
		break;
	case 763:
		str = _T("不能启动 Internet 连接共享。除了共享的连接之外，还有两个或多个局域网连接。");
		break;
	case 764:
		str = _T("未安装智能卡阅读器。");
		break;
	case 765:
		str = _T("不能启动 Internet 连接共享。LAN 连接已经配置了自动填写 IP 地址所需的 IP 地址。");
		break;
	case 766:
		str = _T("系统找不到任一证书。");
		break;
	case 767:
		str = _T("不能启动 Internet 连接共享。专用网络上选择的 LAN 连接配置了多个 IP 地址。");
		break;
	case 768:
		str = _T("由于加密数据失败，导致连接尝试失败。");
		break;
	case 769:
		str = _T("指定的目的地是不可访问的。");
		break;
	case 770:
		str = _T("远程机器拒绝连接尝试。");
		break;
	case 771:
		str = _T("由于网络忙，因此连接尝试失败。");
		break;
	case 772:
		str = _T("远程计算机的网络硬件与请求的电话类型不兼容。");
		break;
	case 773:
		str = _T("由于目标号码已更改，从而导致连接尝试失败。");
		break;
	case 774:
		str = _T("临时故障导致连接尝试失败。");
		break;
	case 775:
		str = _T("呼叫被远程计算机阻塞。");
		break;
	case 776:
		str = _T("由于目标已经调用“请勿打扰”功能，因此该呼叫无法连接。");
		break;
	case 777:
		str = _T("远程计算机上的调制解调器出现故障，导致连接尝试失败。");
		break;
	case 778:
		str = _T("不能验证服务器的身份。");
		break;
	case 779:
		str = _T("使用该连接向外拨号，必须使用智能卡。");
		break;
	case 780:
		str = _T("所尝试使用的功能对此连接无效。");
		break;
	case 781:
		str = _T("由于找不到有效的证书，从而导致加密尝试失败。");
		break;
	case 782:
		str = _T("网络地址转换 (NAT) 当前安装为路由协议，必须在启用 Internet 连接共享前删除。");
		break;
	case 783:
		str = _T("不能启用 Internet 连接共享。选择为专用网络的 LAN 连接不是当前连接，或者已与网络断开连接。");
		break;
	case 784:
		str = _T("登录时无法使用该连接进行拨号，因为它配置使用的用户名和智能卡上的不一样。");
		break;
	case 785:
		str = _T("登录时无法使用该连接进行拨号，因为没有将其配置成使用智能卡。");
		break;
	case 786:
		str = _T("L2TP 连接尝试失败，因为进行安全验证时计算机上没有有效的计算机证书。");
		break;
	case 787:
		str = _T("L2TP 连接尝试失败，因为安全层无法验证远程计算机。");
		break;
	case 788:
		str = _T("L2TP 连接尝试失败，因为安全层无法与远程计算机协商兼容参数。");
		break;
	case 789:
		str = _T("L2TP 连接尝试失败，因为安全层在与远程计算机进行初始协商时遇到处理错误。");
		break;
	case 790:
		str = _T("L2TP 连接尝试失败，因为远程计算机上的证书验证失败。");
		break;
	case 791:
		str = _T("L2TP 连接尝试失败，因为没有找到该连接的安全策略。");
		break;
	case 792:
		str = _T("L2TP 连接尝试失败，因为安全协商超时。");
		break;
	case 793:
		str = _T("L2TP 连接尝试失败，因为协商安全时发生错误。");
		break;
	case 794:
		str = _T("该用户的帧协议 RADIUS 属性不是 PPP。");
		break;
	case 795:
		str = _T("该用户的隧道类型 RADIUS 属性不正确。");
		break;
	case 796:
		str = _T("该用户的服务类型 RADIUS 属性既不是成帧也不是回拨成帧。");
		break;
	case 797:
		str = _T("无法建立到远程计算机的连接，因为找不到调制解调器或者调制解调器忙。");
		break;
	case 798:
		str = _T("找不到可与可扩展身份验证协议一起使用的证书。");
		break;
	case 799:
		str = _T("由于网络上存在 IP 地址冲突，因此无法启用 Internet 连接共享 (ICS)。");
		break;
	case 800:
		str = _T("无法建立 VPN 连接。");
		break;
	default:
		str = _T("未知错误");
	}
	CString strr;
	strr.Format(_T("%u %s"), errCode, str);

	return strr;
}
