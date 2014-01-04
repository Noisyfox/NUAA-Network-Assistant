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
		str = _T("��������");
		break;
	case 601:
		str = _T("��⵽��Ч�Ķ˿ھ����");
		break;
	case 602:
		str = _T("ָ���Ķ˿��Ѵ򿪡�");
		break;
	case 603:
		str = _T("�����˵Ļ�����̫С��");
		break;
	case 604:
		str = _T("ָ���˲���ȷ����Ϣ��");
		break;
	case 605:
		str = _T("�������ö˿���Ϣ��");
		break;
	case 606:
		str = _T("ָ���Ķ˿�δ���ӡ�");
		break;
	case 607:
		str = _T("��⵽��Ч�¼���");
		break;
	case 608:
		str = _T("ָ�����豸�����ڡ�");
		break;
	case 609:
		str = _T("ָ�����豸���Ͳ����ڡ�");
		break;
	case 610:
		str = _T("ָ���Ļ�������Ч��");
		break;
	case 611:
		str = _T("ָ����·�ɲ����á�");
		break;
	case 612:
		str = _T("ָ����·��δ���䡣");
		break;
	case 613:
		str = _T("ָ����ѹ����Ч��");
		break;
	case 614:
		str = _T("û���㹻�Ļ��������á�");
		break;
	case 615:
		str = _T("δ�ҵ�ָ���Ķ˿ڡ�");
		break;
	case 616:
		str = _T("�첽�������");
		break;
	case 617:
		str = _T("���ƽ�����Ѿ��Ͽ����ӡ�");
		break;
	case 618:
		str = _T("ָ���Ķ˿�δ�򿪡�");
		break;
	case 619:
		str = _T("ָ���Ķ˿�δ���ӡ�");
		break;
	case 620:
		str = _T("�޷������˵㡣");
		break;
	case 621:
		str = _T("ϵͳ�޷��򿪵绰����");
		break;
	case 622:
		str = _T("ϵͳ�޷����ص绰����");
		break;
	case 623:
		str = _T("ϵͳ�޷��ҵ������ӵĵ绰���");
		break;
	case 624:
		str = _T("ϵͳ�޷����µ绰���ļ���");
		break;
	case 625:
		str = _T("ϵͳ�ڵ绰�����ҵ���Ч��Ϣ��");
		break;
	case 626:
		str = _T("�޷������ַ�����");
		break;
	case 627:
		str = _T("�޷��ҵ��ؼ��֡�");
		break;
	case 628:
		str = _T("���ӱ��رա�");
		break;
	case 629:
		str = _T("���ӱ�Զ�̼�����رա�");
		break;
	case 630:
		str = _T("����Ӳ�����ϣ����ƽ�����Ͽ����ӡ�");
		break;
	case 631:
		str = _T("�û��Ͽ��˵��ƽ�������ӡ�");
		break;
	case 632:
		str = _T("��⵽����ȷ�Ľṹ��С��");
		break;
	case 633:
		str = _T("���ƽ��������ʹ�û�û������Ϊ������");
		break;
	case 634:
		str = _T("���ļ�����޷���Զ��������ע�ᡣ");
		break;
	case 635:
		str = _T("����δ֪����");
		break;
	case 636:
		str = _T("���ӵ��˿ڵ��豸�������������豸��");
		break;
	case 637:
		str = _T("��⵽����ת�����ַ�����");
		break;
	case 638:
		str = _T("����ʱ��");
		break;
	case 639:
		str = _T("�첽���粻���á�");
		break;
	case 640:
		str = _T("������ NetBIOS �йصĴ���");
		break;
	case 641:
		str = _T("���������ܷ���֧�ֿͻ�������� NetBIOS ��Դ��");
		break;
	case 642:
		str = _T("�������ĳ�� NetBIOS ���Ѿ���Զ��������ע�ᡣ");
		break;
	case 643:
		str = _T("�������˵��������ֹ��ϡ�");
		break;
	case 644:
		str = _T("�����޷��������絯��ʽ��Ϣ��");
		break;
	case 645:
		str = _T("�����ڲ������֤����");
		break;
	case 646:
		str = _T("��ʱ�䲻������ʻ���¼��");
		break;
	case 647:
		str = _T("���ʻ������á�");
		break;
	case 648:
		str = _T("���ʻ��������ѹ��ڡ�");
		break;
	case 649:
		str = _T("�ʻ�û�в����Ȩ�ޡ�");
		break;
	case 650:
		str = _T("Զ�̷��ʷ�����û����Ӧ��");
		break;
	case 651:
		str = _T("���ƽ�����������");
		break;
	case 652:
		str = _T("��һ�����Ե��ƽ�������޷�ʶ�����Ӧ��");
		break;
	case 653:
		str = _T("���豸 .INF �ļ�������δ�ҵ����ƽ����������ĺꡣ");
		break;
	case 654:
		str = _T("�豸 .INF �ļ������е��������Ӧ������δ����ĺꡣ");
		break;
	case 655:
		str = _T("���豸 .INF �ļ�������δ�ҵ� <MESSAGE> �ꡣ");
		break;
	case 656:
		str = _T("���豸 .INF �ļ������е� <DEFAULTOFF> �����δ����ĺꡣ");
		break;
	case 657:
		str = _T("�޷����豸 .INF �ļ���");
		break;
	case 658:
		str = _T("���豸 .INF ��ý�� .INI �ļ��е��豸��̫����");
		break;
	case 659:
		str = _T("ý�� .INI �ļ�������δ֪���豸����");
		break;
	case 660:
		str = _T("�豸 .INF �ļ����������������Ӧ��");
		break;
	case 661:
		str = _T("�豸 .INF �ļ�ȱ�����");
		break;
	case 662:
		str = _T("��ͼ�����豸 .INF �ļ�����û���г��ĺꡣ");
		break;
	case 663:
		str = _T("ý�� .INI �ļ�������δ֪���豸���͡�");
		break;
	case 664:
		str = _T("ϵͳ�ڴ治�㡣");
		break;
	case 665:
		str = _T("δ��ȷ���õ��ƽ������");
		break;
	case 666:
		str = _T("���ƽ����δ����������");
		break;
	case 667:
		str = _T("ϵͳ���ܶ�ȡý�� .INI �ļ���");
		break;
	case 668:
		str = _T("���ӱ���ֹ��");
		break;
	case 669:
		str = _T("ý�� .INI �ļ��е��÷�������Ч��");
		break;
	case 670:
		str = _T("ϵͳ���ܴ�ý�� .INI �ļ��ж�ȡ�������ơ�");
		break;
	case 671:
		str = _T("ϵͳ���ܴ�ý�� .INI �ļ��ж�ȡ�豸���͡�");
		break;
	case 672:
		str = _T("ϵͳ���ܴ�ý�� .INI �ļ��ж�ȡ�豸���ơ�");
		break;
	case 673:
		str = _T("ϵͳ���ܴ�ý�� .INI �ļ��ж�ȡ�÷���");
		break;
	case 674:
		str = _T("ϵͳ���ܴ�ý�� .INI �ļ��ж�ȡ������������ (bps)��");
		break;
	case 675:
		str = _T("ϵͳ���ܴ�ý�� .INI �ļ��ж�ȡ�����ز������ٶȡ�");
		break;
	case 676:
		str = _T("�绰��æ��");
		break;
	case 677:
		str = _T("�����˶����ǵ��ƽ����Ӧ��");
		break;
	case 678:
		str = _T("û��Ӧ��");
		break;
	case 679:
		str = _T("ϵͳ�޷�����ز���");
		break;
	case 680:
		str = _T("û�в�������");
		break;
	case 681:
		str = _T("���ƽ��������һ�����");
		break;
	case 691:
		str = _T("��Ϊ�û�����/�������ڴ�������Ч�����Է��ʱ��ܾ���");
		break;
	case 692:
		str = _T("���ƽ��������Ӳ�����ϡ�");
		break;
	case 695:
		str = _T("δ����״̬������");
		break;
	case 696:
		str = _T("������״̬������");
		break;
	case 697:
		str = _T("��Ӧѭ��δ��ɡ�");
		break;
	case 699:
		str = _T("���ƽ��������Ӧ���»����������");
		break;
	case 700:
		str = _T("�豸 .INF �ļ��е���չ����̫����");
		break;
	case 701:
		str = _T("���ƽ����ʹ���� COM ��������֧�ֵ������ٶȡ�");
		break;
	case 703:
		str = _T("������Ҫ�û���Ϣ����Ӧ�ó��������û�������");
		break;
	case 704:
		str = _T("�ز�������Ч��");
		break;
	case 705:
		str = _T("�����֤״̬��Ч��");
		break;
	case 707:
		str = _T("������ X.25 Э���йصĴ���");
		break;
	case 708:
		str = _T("�ʻ����ڡ�");
		break;
	case 709:
		str = _T("�������ϵ�����ʱ��������");
		break;
	case 710:
		str = _T("������ƽ����ͨѶʱ��⵽�����������");
		break;
	case 711:
		str = _T("Զ�̷��ʷ���������޷������¼���־���ṩ��������Ϣ��");
		break;
	case 712:
		str = _T("˫·�˿����ڳ�ʼ����");
		break;
	case 713:
		str = _T("û�л�� ISDN ��·���á�");
		break;
	case 714:
		str = _T("û�� ISDN �ŵ������ڲ��š�");
		break;
	case 715:
		str = _T("���ڵ绰����������Է����������");
		break;
	case 716:
		str = _T("Զ�̷��ʷ��� IP ���ò����á�");
		break;
	case 717:
		str = _T("��Զ�̷��ʷ��� IP ��ַ�ľ�̬����û�� IP ��ַ���á�");
		break;
	case 718:
		str = _T("�ȴ�Զ�̼������Ч��Ӧ�����ӳ�ʱ��");
		break;
	case 719:
		str = _T("���ӱ�Զ�̼������ֹ��");
		break;
	case 720:
		str = _T("�������ļ������Զ�̼������ PPP ����Э�鲻һ�£��������ӳ���ʧ�ܡ�");
		break;
	case 721:
		str = _T("Զ�̼����û����Ӧ��");
		break;
	case 722:
		str = _T("��Զ�̼�������յ���Ч�����ݡ�");
		break;
	case 723:
		str = _T("�绰���루����ǰ׺�ͺ�׺��̫����");
		break;
	case 726:
		str = _T("IPX Э�鲻�������ڶ���ĵ��ƽ������ͬʱ���Ⲧ�š�");
		break;
	case 728:
		str = _T("ϵͳ�Ҳ��� IP ��������");
		break;
	case 729:
		str = _T("���ǰ�װ IP Э�飬������ʹ�� SLIP��");
		break;
	case 731:
		str = _T("δ����Э�顣");
		break;
	case 732:
		str = _T("���ļ������Զ�̼������ PPP ����Э���޷�һ�¡�");
		break;
	case 733:
		str = _T("���ļ������Զ�̼������ PPP ����Э���޷�һ�¡�");
		break;
	case 734:
		str = _T("PPP ���ӿ���Э�鱻��ֹ��");
		break;
	case 735:
		str = _T("����ĵ�ַ���������ܾ���");
		break;
	case 736:
		str = _T("Զ�̼������ֹ�˿���Э�顣");
		break;
	case 737:
		str = _T("��⵽���ء�");
		break;
	case 738:
		str = _T("������û��ָ�ɵ�ַ��");
		break;
	case 739:
		str = _T("Զ�̷���������������֤Э�鲻��ʹ�ô洢�����롣");
		break;
	case 740:
		str = _T("��⵽��Ч���Ź���");
		break;
	case 741:
		str = _T("���ؼ������֧����������ݼ������͡�");
		break;
	case 742:
		str = _T("Զ�̼������֧����������ݼ������͡�");
		break;
	case 743:
		str = _T("Զ�̷�����Ҫ�����ݼ��ܡ�");
		break;
	case 751:
		str = _T("�ز����������Ч���ַ���");
		break;
	case 752:
		str = _T("������ű�ʱ�����﷨����");
		break;
	case 753:
		str = _T("�����������ɶ�Э��·���������ģ���˸������޷��Ͽ���");
		break;
	case 754:
		str = _T("ϵͳ�޷��ҵ�����·�󶨡�");
		break;
	case 755:
		str = _T("���ڸ����Ѿ�ָ���Զ���Ĳ��ų������ϵͳ����ִ���Զ����š�");
		break;
	case 756:
		str = _T("�Ѿ���������ӡ�");
		break;
	case 757:
		str = _T("Զ�̷��ʷ������Զ��������¼���־���ṩ��������Ϣ��");
		break;
	case 758:
		str = _T("���������Ѿ����� Internet ���ӹ���");
		break;
	case 760:
		str = _T("����·�ɹ���ʱ��������");
		break;
	case 761:
		str = _T("�������ӵ� Internet ���ӹ���ʱ��������");
		break;
	case 763:
		str = _T("�������� Internet ���ӹ������˹��������֮�⣬���������������������ӡ�");
		break;
	case 764:
		str = _T("δ��װ���ܿ��Ķ�����");
		break;
	case 765:
		str = _T("�������� Internet ���ӹ���LAN �����Ѿ��������Զ���д IP ��ַ����� IP ��ַ��");
		break;
	case 766:
		str = _T("ϵͳ�Ҳ�����һ֤�顣");
		break;
	case 767:
		str = _T("�������� Internet ���ӹ���ר��������ѡ��� LAN ���������˶�� IP ��ַ��");
		break;
	case 768:
		str = _T("���ڼ�������ʧ�ܣ��������ӳ���ʧ�ܡ�");
		break;
	case 769:
		str = _T("ָ����Ŀ�ĵ��ǲ��ɷ��ʵġ�");
		break;
	case 770:
		str = _T("Զ�̻����ܾ����ӳ��ԡ�");
		break;
	case 771:
		str = _T("��������æ��������ӳ���ʧ�ܡ�");
		break;
	case 772:
		str = _T("Զ�̼����������Ӳ��������ĵ绰���Ͳ����ݡ�");
		break;
	case 773:
		str = _T("����Ŀ������Ѹ��ģ��Ӷ��������ӳ���ʧ�ܡ�");
		break;
	case 774:
		str = _T("��ʱ���ϵ������ӳ���ʧ�ܡ�");
		break;
	case 775:
		str = _T("���б�Զ�̼����������");
		break;
	case 776:
		str = _T("����Ŀ���Ѿ����á�������š����ܣ���˸ú����޷����ӡ�");
		break;
	case 777:
		str = _T("Զ�̼�����ϵĵ��ƽ�������ֹ��ϣ��������ӳ���ʧ�ܡ�");
		break;
	case 778:
		str = _T("������֤����������ݡ�");
		break;
	case 779:
		str = _T("ʹ�ø��������Ⲧ�ţ�����ʹ�����ܿ���");
		break;
	case 780:
		str = _T("������ʹ�õĹ��ܶԴ�������Ч��");
		break;
	case 781:
		str = _T("�����Ҳ�����Ч��֤�飬�Ӷ����¼��ܳ���ʧ�ܡ�");
		break;
	case 782:
		str = _T("�����ַת�� (NAT) ��ǰ��װΪ·��Э�飬���������� Internet ���ӹ���ǰɾ����");
		break;
	case 783:
		str = _T("�������� Internet ���ӹ���ѡ��Ϊר������� LAN ���Ӳ��ǵ�ǰ���ӣ�������������Ͽ����ӡ�");
		break;
	case 784:
		str = _T("��¼ʱ�޷�ʹ�ø����ӽ��в��ţ���Ϊ������ʹ�õ��û��������ܿ��ϵĲ�һ����");
		break;
	case 785:
		str = _T("��¼ʱ�޷�ʹ�ø����ӽ��в��ţ���Ϊû�н������ó�ʹ�����ܿ���");
		break;
	case 786:
		str = _T("L2TP ���ӳ���ʧ�ܣ���Ϊ���а�ȫ��֤ʱ�������û����Ч�ļ����֤�顣");
		break;
	case 787:
		str = _T("L2TP ���ӳ���ʧ�ܣ���Ϊ��ȫ���޷���֤Զ�̼������");
		break;
	case 788:
		str = _T("L2TP ���ӳ���ʧ�ܣ���Ϊ��ȫ���޷���Զ�̼����Э�̼��ݲ�����");
		break;
	case 789:
		str = _T("L2TP ���ӳ���ʧ�ܣ���Ϊ��ȫ������Զ�̼�������г�ʼЭ��ʱ�����������");
		break;
	case 790:
		str = _T("L2TP ���ӳ���ʧ�ܣ���ΪԶ�̼�����ϵ�֤����֤ʧ�ܡ�");
		break;
	case 791:
		str = _T("L2TP ���ӳ���ʧ�ܣ���Ϊû���ҵ������ӵİ�ȫ���ԡ�");
		break;
	case 792:
		str = _T("L2TP ���ӳ���ʧ�ܣ���Ϊ��ȫЭ�̳�ʱ��");
		break;
	case 793:
		str = _T("L2TP ���ӳ���ʧ�ܣ���ΪЭ�̰�ȫʱ��������");
		break;
	case 794:
		str = _T("���û���֡Э�� RADIUS ���Բ��� PPP��");
		break;
	case 795:
		str = _T("���û���������� RADIUS ���Բ���ȷ��");
		break;
	case 796:
		str = _T("���û��ķ������� RADIUS ���ԼȲ��ǳ�֡Ҳ���ǻز���֡��");
		break;
	case 797:
		str = _T("�޷�������Զ�̼���������ӣ���Ϊ�Ҳ������ƽ�������ߵ��ƽ����æ��");
		break;
	case 798:
		str = _T("�Ҳ����������չ�����֤Э��һ��ʹ�õ�֤�顣");
		break;
	case 799:
		str = _T("���������ϴ��� IP ��ַ��ͻ������޷����� Internet ���ӹ��� (ICS)��");
		break;
	case 800:
		str = _T("�޷����� VPN ���ӡ�");
		break;
	default:
		str = _T("δ֪����");
	}
	CString strr;
	strr.Format(_T("%u %s"), errCode, str);

	return strr;
}
