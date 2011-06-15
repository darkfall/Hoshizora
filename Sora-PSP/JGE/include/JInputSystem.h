#ifndef _JINPUTSYSTEM_H_
#define _JINPUTSYSTEM_H_

#include "JLBFont.h"

extern char input_table[3][9][4];

struct PY_index
	{
		char *PY;
		char *PY_mb;
	};
//"ƴ�����뷨�������б�,���(mb)"
static char PY_mb_a[]     ={"�����"};
static char PY_mb_ai[]    ={"��������������������������"};
static char PY_mb_an[]    ={"������������������"};
static char PY_mb_ang[]   ={"������"};
static char PY_mb_ao[]    ={"�������������°İ�"};
static char PY_mb_ba[]    ={"�˰ͰȰǰɰṴ̋ưʰΰϰѰаӰְհ�"};
static char PY_mb_bai[]   ={"�װٰ۰ذڰܰݰ�"};
static char PY_mb_ban[]   ={"�����߰����������"};
static char PY_mb_bang[]  ={"������������������"};
static char PY_mb_bao[]   ={"������������������������������������"};
static char PY_mb_bei[]   ={"������������������������������"};
static char PY_mb_ben[]   ={"����������"};
static char PY_mb_beng[]  ={"�����±ñű�"};
static char PY_mb_bi[]    ={"�ƱǱȱ˱ʱɱұرϱձӱѱݱбֱԱͱױ̱αڱܱ�"};
static char PY_mb_bian[]  ={"�߱�ޱ���������"};
static char PY_mb_biao[]  ={"�����"};
static char PY_mb_bie[]   ={"�����"};
static char PY_mb_bin[]   ={"����������"};
static char PY_mb_bing[]  ={"������������������"};
static char PY_mb_bo[]    ={"����������������������������������������"};
static char PY_mb_bu[]    ={"��������������������"};
static char PY_mb_ca[]    ={"��"};
static char PY_mb_cai[]   ={"�²ŲĲƲòɲʲǲȲ˲�"};
static char PY_mb_can[]   ={"�βͲвϲѲҲ�"};
static char PY_mb_cang[]  ={"�ֲײԲղ�"};
static char PY_mb_cao[]   ={"�ٲڲܲ۲�"};
static char PY_mb_ce[]    ={"���޲��"};
static char PY_mb_ceng[]  ={"�����"};
static char PY_mb_cha[]   ={"������������ɲ"};
static char PY_mb_chai[]  ={"����"};
static char PY_mb_chan[]  ={"�������������������"};
static char PY_mb_chang[] ={"������������������������"};
static char PY_mb_chao[]  ={"��������������������"};
static char PY_mb_che[]   ={"������������"};
static char PY_mb_chen[]  ={"�������������³��ĳ�"};
static char PY_mb_cheng[] ={"�Ƴųɳʳгϳǳ˳ͳ̳γȳѳҳ�"};
static char PY_mb_chi[]   ={"�Գճڳس۳ٳֳ߳޳ݳܳ����"};
static char PY_mb_chong[] ={"������"};
static char PY_mb_chou[]  ={"�������������"};
static char PY_mb_chu[]   ={"����������������������������������"};
static char PY_mb_chuai[] ={"��"};
static char PY_mb_chuan[] ={"��������������"};
static char PY_mb_chuang[]={"����������"};
static char PY_mb_chui[]  ={"����������"};
static char PY_mb_chun[]  ={"��������������"};
static char PY_mb_chuo[]  ={"��"};
static char PY_mb_ci[]    ={"�ôʴĴɴȴǴŴƴ˴δ̴�"};
static char PY_mb_cong[]  ={"�ѴӴҴдϴ�"};
static char PY_mb_cou[]   ={"��"};
static char PY_mb_cu[]    ={"�ִٴ״�"};
static char PY_mb_cuan[]  ={"�ڴܴ�"};
static char PY_mb_cui[]   ={"�޴ߴݴ�����"};
static char PY_mb_cun[]   ={"����"};
static char PY_mb_cuo[]   ={"�������"};
static char PY_mb_da[]    ={"�������"};
static char PY_mb_dai[]   ={"������������������������"};
static char PY_mb_dan[]   ={"������������������������������"};
static char PY_mb_dang[]  ={"����������"};
static char PY_mb_dao[]   ={"������������������������"};
static char PY_mb_de[]    ={"�õµ�"};
static char PY_mb_deng[]  ={"�Ƶǵŵȵ˵ʵ�"};
static char PY_mb_di[]    ={"�͵̵εҵϵеӵѵյ׵ֵصܵ۵ݵڵ޵�"};
static char PY_mb_dian[]  ={"���ߵ�������������"};
static char PY_mb_diao[]  ={"�����������"};
static char PY_mb_die[]   ={"��������������"};
static char PY_mb_ding[]  ={"������������������"};
static char PY_mb_diu[]   ={"��"};
static char PY_mb_dong[]  ={"��������������������"};
static char PY_mb_dou[]   ={"����������������"};
static char PY_mb_du[]    ={"�����������¶ĶöʶŶǶȶɶ�"};
static char PY_mb_duan[]  ={"�˶̶ζ϶ж�"};
static char PY_mb_dui[]   ={"�ѶӶԶ�"};
static char PY_mb_dun[]   ={"�ֶضն׶ܶ۶ٶ�"};
static char PY_mb_duo[]   ={"��߶�޶��������"};
static char PY_mb_e[]     ={"����������������"};
static char PY_mb_en[]    ={"��"};
static char PY_mb_er[]    ={"����������������"};
static char PY_mb_fa[]    ={"����������������"};
static char PY_mb_fan[]   ={"����������������������������������"};
static char PY_mb_fang[]  ={"���������������·÷ķ�"};
static char PY_mb_fei[]   ={"�ɷǷȷƷʷ˷̷ͷϷзη�"};
static char PY_mb_fen[]   ={"�ַԷ׷ҷշӷطڷٷ۷ݷܷ޷߷�"};
static char PY_mb_feng[]  ={"����������������"};
static char PY_mb_fo[]    ={"��"};
static char PY_mb_fou[]   ={"��"};
static char PY_mb_fu[]    ={"������󸥷�����������������������������������������������������������������������������"};
static char PY_mb_ga[]    ={"�¸�"};
static char PY_mb_gai[]   ={"�øĸƸǸȸ�"};
static char PY_mb_gan[]   ={"�ɸʸ˸θ̸͸ѸϸҸи�"};
static char PY_mb_gang[]  ={"�Ըոڸٸظ׸ָ۸�"};
static char PY_mb_gao[]   ={"�޸�߸�ݸ�����"};
static char PY_mb_ge[]    ={"����������������������"};
static char PY_mb_gei[]   ={"��"};
static char PY_mb_gen[]   ={"����"};
static char PY_mb_geng[]  ={"��������������"};
static char PY_mb_gong[]  ={"������������������������������"};
static char PY_mb_gou[]   ={"������������������"};
static char PY_mb_gu[]    ={"�����ù¹��������ŹȹɹǹƹĹ̹ʹ˹�"};
static char PY_mb_gua[]   ={"�Ϲιйѹҹ�"};
static char PY_mb_guai[]  ={"�Թչ�"};
static char PY_mb_guan[]  ={"�ع۹ٹڹ׹ݹܹ�߹��"};
static char PY_mb_guang[] ={"����"};
static char PY_mb_gui[]   ={"������������������"};
static char PY_mb_gun[]   ={"������"};
static char PY_mb_guo[]   ={"������������"};
static char PY_mb_ha[]    ={"���"};
static char PY_mb_hai[]   ={"��������������"};
static char PY_mb_han[]   ={"��������������������������������������"};
static char PY_mb_hang[]  ={"������"};
static char PY_mb_hao[]   ={"���������úºźƺ�"};
static char PY_mb_he[]    ={"�ǺȺ̺ϺκͺӺҺ˺ɺԺкʺغֺպ�"};
static char PY_mb_hei[]   ={"�ں�"};
static char PY_mb_hen[]   ={"�ۺܺݺ�"};
static char PY_mb_heng[]  ={"��ߺ���"};
static char PY_mb_hong[]  ={"����������"};
static char PY_mb_hou[]   ={"��������"};
static char PY_mb_hu[]    ={"������������������������������������"};
static char PY_mb_hua[]   ={"������������������"};
static char PY_mb_huai[]  ={"����������"};
static char PY_mb_huan[]  ={"�����������û»�������������"};
static char PY_mb_huang[] ={"�ĻŻʻ˻ƻ̻ͻȻǻɻлλѻ�"};
static char PY_mb_hui[]   ={"�һֻӻԻջػ׻ڻܻ������߻޻�ݻٻ�"};
static char PY_mb_hun[]   ={"�������"};
static char PY_mb_huo[]   ={"�������������"};
static char PY_mb_ji[]    ={"���������������������������������������������������������������������ƼǼ��ͼ˼ɼ��ʼ����ȼü̼żļ¼�������"};
static char PY_mb_jia[]   ={"�ӼмѼϼҼμԼռ׼ּؼۼݼܼټ޼�Ю"};
static char PY_mb_jian[]  ={"����߼����������������������������������������������������"};
static char PY_mb_jiang[] ={"��������������������������"};
static char PY_mb_jiao[]  ={"���������������������ǽƽʽȽýŽ½��˽ɽнνϽ̽ѽ;���"};
static char PY_mb_jie[]   ={"�׽Խӽսҽֽڽٽܽ��ݽ޽ؽ߽����������"};
static char PY_mb_jin[]   ={"���������������������������������"};
static char PY_mb_jing[]  ={"��������������������������������������������������"};
static char PY_mb_jiong[] ={"����"};
static char PY_mb_jiu[]   ={"�������žþľ��¾ƾɾʾ̾ξǾȾ;�"};
static char PY_mb_ju[]    ={"�ӾоѾԾҾϾֽ۾վ׾ھپؾ�޾ܾ߾����ݾ��۾�"};
static char PY_mb_juan[]  ={"��������"};
static char PY_mb_jue[]   ={"��������������"};
static char PY_mb_jun[]   ={"����������������������"};
static char PY_mb_ka[]    ={"������"};
static char PY_mb_kai[]   ={"����������"};
static char PY_mb_kan[]   ={"��������������"};
static char PY_mb_kang[]  ={"��������������"};
static char PY_mb_kao[]   ={"��������"};
static char PY_mb_ke[]    ={"�����¿ƿÿſĿǿȿɿʿ˿̿Ϳ�"};
static char PY_mb_ken[]   ={"�Ͽѿҿ�"};
static char PY_mb_keng[]  ={"�Կ�"};
static char PY_mb_kong[]  ={"�տ׿ֿ�"};
static char PY_mb_kou[]   ={"�ٿڿۿ�"};
static char PY_mb_ku[]    ={"�ݿ޿߿����"};
static char PY_mb_kua[]   ={"������"};
static char PY_mb_kuai[]  ={"�����"};
static char PY_mb_kuan[]  ={"���"};
static char PY_mb_kuang[] ={"�����������"};
static char PY_mb_kui[]   ={"����������������������"};
static char PY_mb_kun[]   ={"��������"};
static char PY_mb_kuo[]   ={"��������"};
static char PY_mb_la[]    ={"��������������"};
static char PY_mb_lai[]   ={"������"};
static char PY_mb_lan[]   ={"������������������������������"};
static char PY_mb_lang[]  ={"����������������"};
static char PY_mb_lao[]   ={"������������������"};
static char PY_mb_le[]    ={"������"};
static char PY_mb_lei[]   ={"����������������������"};
static char PY_mb_leng[]  ={"������"};
static char PY_mb_li[]    ={"��������������������������������������������������������������������"};
static char PY_mb_lian[]  ={"����������������������������"};
static char PY_mb_liang[] ={"������������������������"};
static char PY_mb_liao[]  ={"������������������������"};
static char PY_mb_lie[]   ={"����������"};
static char PY_mb_lin[]   ={"������������������������"};
static char PY_mb_ling[]  ={"����������������������������"};
static char PY_mb_liu[]   ={"����������������������"};
static char PY_mb_long[]  ={"��������¡��¤¢£"};
static char PY_mb_lou[]   ={"¦¥§¨ª©"};
static char PY_mb_lu[]    ={"¶¬®«¯­±²°³½¼¸¹»µ·¾º´"};
static char PY_mb_luan[]  ={"������������"};
static char PY_mb_lue[]   ={"����"};
static char PY_mb_lun[]   ={"��������������"};
static char PY_mb_luo[]   ={"������������������������"};
static char PY_mb_lv[]    ={"��¿������������������������"};
static char PY_mb_ma[]    ={"������������������"};
static char PY_mb_mai[]   ={"������������"};
static char PY_mb_man[]   ={"����������á������"};
static char PY_mb_mang[]  ={"æâäãçå"};
static char PY_mb_mao[]   ={"èëìéêîíïðóñò"};
static char PY_mb_me[]    ={"ô"};
static char PY_mb_mei[]   ={"ûöõü÷ýúøùÿ��þ��������"};
static char PY_mb_men[]   ={"������"};
static char PY_mb_meng[]  ={"����������������"};
static char PY_mb_mi[]    ={"����������������������������"};
static char PY_mb_mian[]  ={"������������������"};
static char PY_mb_miao[]  ={"����������������"};
static char PY_mb_mie[]   ={"����"};
static char PY_mb_min[]   ={"������������"};
static char PY_mb_ming[]  ={"������������"};
static char PY_mb_miu[]   ={"��"};
static char PY_mb_mo[]    ={"����ġģĤĦĥĢħĨĩĭİĪįĮīĬ"};
static char PY_mb_mou[]   ={"Ĳıĳ"};
static char PY_mb_mu[]    ={"ĸĶĵķĴľĿ��ļĹĻ��Ľĺ��"};
static char PY_mb_na[]    ={"��������������"};
static char PY_mb_nai[]   ={"����������"};
static char PY_mb_nan[]   ={"������"};
static char PY_mb_nang[]  ={"��"};
static char PY_mb_nao[]   ={"����������"};
static char PY_mb_ne[]    ={"��"};
static char PY_mb_nei[]   ={"����"};
static char PY_mb_nen[]   ={"��"};
static char PY_mb_neng[]  ={"��"};
static char PY_mb_ni[]    ={"����������������������"};
static char PY_mb_nian[]  ={"��������������"};
static char PY_mb_niang[] ={"����"};
static char PY_mb_niao[]  ={"����"};
static char PY_mb_nie[]   ={"��������������"};
static char PY_mb_nin[]   ={"��"};
static char PY_mb_ning[]  ={"��š������Ţ"};
static char PY_mb_niu[]   ={"ţŤŦť"};
static char PY_mb_nong[]  ={"ũŨŧŪ"};
static char PY_mb_nu[]    ={"ūŬŭ"};
static char PY_mb_nuan[]  ={"ů"};
static char PY_mb_nue[]   ={"űŰ"};
static char PY_mb_nuo[]   ={"ŲŵųŴ"};
static char PY_mb_nv[]    ={"Ů"};
static char PY_mb_o[]     ={"Ŷ"};
static char PY_mb_ou[]    ={"ŷŹŸŻżźŽ"};
static char PY_mb_pa[]    ={"ſž����������"};
static char PY_mb_pai[]   ={"������������"};
static char PY_mb_pan[]   ={"����������������"};
static char PY_mb_pang[]  ={"����������"};
static char PY_mb_pao[]   ={"��������������"};
static char PY_mb_pei[]   ={"������������������"};
static char PY_mb_pen[]   ={"����"};
static char PY_mb_peng[]  ={"����������������������������"};
static char PY_mb_pi[]    ={"��������������Ƥ��ƣơ��ƢƥƦƨƧƩ"};
static char PY_mb_pian[]  ={"Ƭƫƪƭ"};
static char PY_mb_piao[]  ={"ƯƮưƱ"};
static char PY_mb_pie[]   ={"ƲƳ"};
static char PY_mb_pin[]   ={"ƴƶƵƷƸ"};
static char PY_mb_ping[]  ={"ƹƽ��ƾƺƻ��ƿƼ"};
static char PY_mb_po[]    ={"����������������"};
static char PY_mb_pou[]   ={"��"};
static char PY_mb_pu[]    ={"������������������������������"};
static char PY_mb_qi[]    ={"������������������������������������������������������������������������"};
static char PY_mb_qia[]   ={"��ǡǢ"};
static char PY_mb_qian[]  ={"ǧǪǤǨǥǣǦǫǩǰǮǯǬǱǭǳǲǴǷǵǶǸ"};
static char PY_mb_qiang[] ={"ǺǼǹǻǿǽǾ��"};
static char PY_mb_qiao[]  ={"������������������������������"};
static char PY_mb_qie[]   ={"����������"};
static char PY_mb_qin[]   ={"����������������������"};
static char PY_mb_qing[]  ={"��������������������������"};
static char PY_mb_qiong[] ={"����"};
static char PY_mb_qiu[]   ={"����������������"};
static char PY_mb_qu[]    ={"����������������ȡȢȣȥȤ"};
static char PY_mb_quan[]  ={"ȦȫȨȪȭȬȩȧȮȰȯ"};
static char PY_mb_que[]   ={"Ȳȱȳȴȸȷȵȶ"};
static char PY_mb_qun[]   ={"ȹȺ"};
static char PY_mb_ran[]   ={"ȻȼȽȾ"};
static char PY_mb_rang[]  ={"ȿ��������"};
static char PY_mb_rao[]   ={"������"};
static char PY_mb_re[]    ={"����"};
static char PY_mb_ren[]   ={"��������������������"};
static char PY_mb_reng[]  ={"����"};
static char PY_mb_ri[]    ={"��"};
static char PY_mb_rong[]  ={"��������������������"};
static char PY_mb_rou[]   ={"������"};
static char PY_mb_ru[]    ={"��������������������"};
static char PY_mb_ruan[]  ={"����"};
static char PY_mb_rui[]   ={"������"};
static char PY_mb_run[]   ={"����"};
static char PY_mb_ruo[]   ={"����"};
static char PY_mb_sa[]    ={"������"};
static char PY_mb_sai[]   ={"��������"};
static char PY_mb_san[]   ={"����ɡɢ"};
static char PY_mb_sang[]  ={"ɣɤɥ"};
static char PY_mb_sao[]   ={"ɦɧɨɩ"};
static char PY_mb_se[]    ={"ɫɬɪ"};
static char PY_mb_sen[]   ={"ɭ"};
static char PY_mb_seng[]  ={"ɮ"};
static char PY_mb_sha[]   ={"ɱɳɴɰɯɵɶɷ��"};
static char PY_mb_shai[]  ={"ɸɹ"};
static char PY_mb_shan[]  ={"ɽɾɼ��ɺɿ������ɻ������������դ"};
static char PY_mb_shang[] ={"����������������"};
static char PY_mb_shao[]  ={"����������������������"};
static char PY_mb_she[]   ={"������������������������"};
static char PY_mb_shen[]  ={"��������������������������������ʲ"};
static char PY_mb_sheng[] ={"��������ʤ����ʡʥʢʣ"};
static char PY_mb_shi[]   ={"��ʬʧʦʭʫʩʨʪʮʯʱʶʵʰʴʳʷʸʹʼʻʺʿ��������ʾʽ������������������������������������"};
static char PY_mb_shou[]  ={"��������������������"};
static char PY_mb_shu[]   ={"������������������������������������������������������ˡ����������"};
static char PY_mb_shua[]  ={"ˢˣ"};
static char PY_mb_shuai[] ={"˥ˤ˦˧"};
static char PY_mb_shuan[] ={"˩˨"};
static char PY_mb_shuang[]={"˫˪ˬ"};
static char PY_mb_shui[]  ={"˭ˮ˰˯"};
static char PY_mb_shun[]  ={"˱˳˴˲"};
static char PY_mb_shuo[]  ={"˵˸˷˶"};
static char PY_mb_si[]    ={"˿˾˽˼˹˻˺����������������"};
static char PY_mb_song[]  ={"����������������"};
static char PY_mb_sou[]   ={"��������"};
static char PY_mb_su[]    ={"����������������������"};
static char PY_mb_suan[]  ={"������"};
static char PY_mb_sui[]   ={"����������������������"};
static char PY_mb_sun[]   ={"������"};
static char PY_mb_suo[]   ={"����������������"};
static char PY_mb_ta[]    ={"����������̡̢̤̣"};
static char PY_mb_tai[]   ={"̨̧̥̦̫̭̬̩̪"};
static char PY_mb_tan[]   ={"̸̵̷̶̴̮̰̯̲̱̳̹̻̺̼̾̿̽"};
static char PY_mb_tang[]  ={"��������������������������"};
static char PY_mb_tao[]   ={"����������������������"};
static char PY_mb_te[]    ={"��"};
static char PY_mb_teng[]  ={"��������"};
static char PY_mb_ti[]    ={"������������������������������"};
static char PY_mb_tian[]  ={"����������������"};
static char PY_mb_tiao[]  ={"������������"};
static char PY_mb_tie[]   ={"������"};
static char PY_mb_ting[]  ={"��͡����ͤͥͣͦͧ͢"};
static char PY_mb_tong[]  ={"ͨͬͮͩͭͯͪͫͳͱͰͲʹ"};
static char PY_mb_tou[]   ={"͵ͷͶ͸"};
static char PY_mb_tu[]    ={"͹ͺͻͼͽͿ;��������"};
static char PY_mb_tuan[]  ={"����"};
static char PY_mb_tui[]   ={"������������"};
static char PY_mb_tun[]   ={"��������"};
static char PY_mb_tuo[]   ={"����������������������"};
static char PY_mb_wa[]    ={"��������������"};
static char PY_mb_wai[]   ={"����"};
static char PY_mb_wan[]   ={"����������������������������������"};
static char PY_mb_wang[]  ={"��������������������"};
static char PY_mb_wei[]   ={"Σ��΢ΡΪΤΧΥΦΨΩάΫΰαβγέίή��δλζηθξνιμεοκ"};
static char PY_mb_wen[]   ={"��������������������"};
static char PY_mb_weng[]  ={"������"};
static char PY_mb_wo[]    ={"������������������"};
static char PY_mb_wu[]    ={"����������������������������������������������������������"};
static char PY_mb_xi[]    ={"Ϧϫ����ϣ������Ϣ��Ϥϧϩ����ϬϡϪ��Ϩ����ϥϰϯϮϱϭϴϲϷϵϸ϶"};
static char PY_mb_xia[]   ={"ϺϹϻ��Ͽ��ϾϽϼ������"};
static char PY_mb_xian[]  ={"ϳ����������������������������������������������������"};
static char PY_mb_xiang[] ={"����������������������������������������"};
static char PY_mb_xiao[]  ={"����������������С��ТФ��ЧУЦХ"};
static char PY_mb_xie[]   ={"ЩШЪЫЭавбгЯЬдйкжмелиз"};
static char PY_mb_xin[]   ={"����о����п��н����"};
static char PY_mb_xing[]  ={"����������������������������"};
static char PY_mb_xiong[] ={"��������������"};
static char PY_mb_xiu[]   ={"��������������������"};
static char PY_mb_xu[]    ={"��������������������������������������"};
static char PY_mb_xuan[]  ={"������������ѡѢѤѣ"};
static char PY_mb_xue[]   ={"��ѥѦѨѧѩѪ"};
static char PY_mb_xun[]   ={"ѫѬѰѲѮѱѯѭѵѶѴѸѷѳ"};
static char PY_mb_ya[]    ={"ѾѹѽѺѻѼ��ѿ����������������"};
static char PY_mb_yan[]   ={"������������������������������������������������������������������"};
static char PY_mb_yang[]  ={"����������������������������������"};
static char PY_mb_yao[]   ={"��������ҢҦҤҥҡң��ҧҨҩҪҫԿ"};
static char PY_mb_ye[]    ={"ҬҭүҮҲұҰҵҶҷҳҹҴҺҸ"};
static char PY_mb_yi[]    ={"һ����ҽ��ҿҼҾ������������������������������������������������������������������������������������������"};
static char PY_mb_yin[]   ={"������������������������������ӡ"};
static char PY_mb_ying[]  ={"ӦӢӤӧӣӥӭӯӫӨөӪӬӮӱӰӳӲ"};
static char PY_mb_yo[]    ={"Ӵ"};
static char PY_mb_yong[]  ={"ӶӵӸӹӺӷ��ӽӾ��ӿ��Ӽӻ��"};
static char PY_mb_you[]   ={"����������������������������������������"};
static char PY_mb_yu[]    ={"����������������������������������������������������Ԧ����������ԡԤ������Ԣ��ԣ������ԥ"};
static char PY_mb_yuan[]  ={"ԩԧԨԪԱ԰ԫԭԲԬԮԵԴԳԯԶԷԹԺԸ"};
static char PY_mb_yue[]   ={"ԻԼ��������Ծ��Խ"};
static char PY_mb_yun[]   ={"������������������������"};
static char PY_mb_za[]    ={"������զ"};
static char PY_mb_zai[]   ={"����������������"};
static char PY_mb_zan[]   ={"��������"};
static char PY_mb_zang[]  ={"������"};
static char PY_mb_zao[]   ={"����������������������������"};
static char PY_mb_ze[]    ={"��������"};
static char PY_mb_zei[]   ={"��"};
static char PY_mb_zen[]   ={"��"};
static char PY_mb_zeng[]  ={"������"};
static char PY_mb_zha[]   ={"����������բագէթըե��"};
static char PY_mb_zhai[]  ={"իժլ��խծկ"};
static char PY_mb_zhan[]  ={"մձճղհնչյոշռսջվ��տպ"};
static char PY_mb_zhang[] ={"���������������������������������"};
static char PY_mb_zhao[]  ={"��������������������צ"};
static char PY_mb_zhe[]   ={"����������������������"};
static char PY_mb_zhen[]  ={"��������������������������������֡"};
static char PY_mb_zheng[] ={"��������������������֤֣��֢"};
static char PY_mb_zhi[]   ={"ְֱֲֳִֵֶַָֹֺֻּֽ֧֥֦֪֭֮֨֯֫֬֩��־������������ֿ������������������"};
static char PY_mb_zhong[] ={"����������������������"};
static char PY_mb_zhou[]  ={"����������������������������"};
static char PY_mb_zhu[]   ={"������������������������������ס��ע��פ��ף��������"};
static char PY_mb_zhua[]  ={"ץ"};
static char PY_mb_zhuai[] ={"ק"};
static char PY_mb_zhuan[] ={"רשת׫׭"};
static char PY_mb_zhuang[]={"ױׯ׮װ׳״��ײ"};
static char PY_mb_zhui[]  ={"׷׵׶׹׺׸"};
static char PY_mb_zhun[]  ={"׻׼"};
static char PY_mb_zhuo[]  ={"׿׾׽��������������"};
static char PY_mb_zi[]    ={"����������������������������"};
static char PY_mb_zong[]  ={"��������������"};
static char PY_mb_zou[]   ={"��������"};
static char PY_mb_zu[]    ={"����������������"};
static char PY_mb_zuan[]  ={"׬����"};
static char PY_mb_zui[]   ={"��������"};
static char PY_mb_zun[]   ={"����"};
static char PY_mb_zuo[]   ={"��������������"};
static char PY_mb_space[] ={""};

/*"ƴ�����뷨��ѯ���,������ĸ������(index)"*/
static struct PY_index  PY_index_a[]={{"",PY_mb_a},
{"i",PY_mb_ai},
{"n",PY_mb_an},
{"ng",PY_mb_ang},
{"o",PY_mb_ao}};
static struct PY_index  PY_index_b[]={{"a",PY_mb_ba},
{"ai",PY_mb_bai},
{"an",PY_mb_ban},
{"ang",PY_mb_bang},
{"ao",PY_mb_bao},
{"ei",PY_mb_bei},
{"en",PY_mb_ben},
{"eng",PY_mb_beng},
{"i",PY_mb_bi},
{"ian",PY_mb_bian},
{"iao",PY_mb_biao},
{"ie",PY_mb_bie},
{"in",PY_mb_bin},
{"ing",PY_mb_bing},
{"o",PY_mb_bo},
{"u",PY_mb_bu}};
static struct PY_index  PY_index_c[]={{"a",PY_mb_ca},
{"ai",PY_mb_cai},
{"an",PY_mb_can},
{"ang",PY_mb_cang},
{"ao",PY_mb_cao},
{"e",PY_mb_ce},
{"eng",PY_mb_ceng},
{"ha",PY_mb_cha},
{"hai",PY_mb_chai},
{"han",PY_mb_chan},
{"hang",PY_mb_chang},
{"hao",PY_mb_chao},
{"he",PY_mb_che},
{"hen",PY_mb_chen},
{"heng",PY_mb_cheng},
{"hi",PY_mb_chi},
{"hong",PY_mb_chong},
{"hou",PY_mb_chou},
{"hu",PY_mb_chu},
{"huai",PY_mb_chuai},
{"huan",PY_mb_chuan},
{"huang",PY_mb_chuang},
{"hui",PY_mb_chui},
{"hun",PY_mb_chun},
{"huo",PY_mb_chuo},
{"i",PY_mb_ci},
{"ong",PY_mb_cong},
{"ou",PY_mb_cou},
{"u",PY_mb_cu},
{"uan",PY_mb_cuan},
{"ui",PY_mb_cui},
{"un",PY_mb_cun},
{"uo",PY_mb_cuo}};
static struct PY_index  PY_index_d[]={{"a",PY_mb_da},
{"ai",PY_mb_dai},
{"an",PY_mb_dan},
{"ang",PY_mb_dang},
{"ao",PY_mb_dao},
{"e",PY_mb_de},
{"eng",PY_mb_deng},
{"i",PY_mb_di},
{"ian",PY_mb_dian},
{"iao",PY_mb_diao},
{"ie",PY_mb_die},
{"ing",PY_mb_ding},
{"iu",PY_mb_diu},
{"ong",PY_mb_dong},
{"ou",PY_mb_dou},
{"u",PY_mb_du},
{"uan",PY_mb_duan},
{"ui",PY_mb_dui},
{"un",PY_mb_dun},
{"uo",PY_mb_duo}};
static struct PY_index  PY_index_e[]={{"",PY_mb_e},
{"n",PY_mb_en},
{"r",PY_mb_er}};
static struct PY_index  PY_index_f[]={{"a",PY_mb_fa},
{"an",PY_mb_fan},
{"ang",PY_mb_fang},
{"ei",PY_mb_fei},
{"en",PY_mb_fen},
{"eng",PY_mb_feng},
{"o",PY_mb_fo},
{"ou",PY_mb_fou},
{"u",PY_mb_fu}};
static struct PY_index  PY_index_g[]={{"a",PY_mb_ga},
{"ai",PY_mb_gai},
{"an",PY_mb_gan},
{"ang",PY_mb_gang},
{"ao",PY_mb_gao},
{"e",PY_mb_ge},
{"ei",PY_mb_gei},
{"en",PY_mb_gen},
{"eng",PY_mb_geng},
{"ong",PY_mb_gong},
{"ou",PY_mb_gou},
{"u",PY_mb_gu},
{"ua",PY_mb_gua},
{"uai",PY_mb_guai},
{"uan",PY_mb_guan},
{"uang",PY_mb_guang},
{"ui",PY_mb_gui},
{"un",PY_mb_gun},
{"uo",PY_mb_guo}};
static struct PY_index  PY_index_h[]={{"a",PY_mb_ha},
{"ai",PY_mb_hai},
{"an",PY_mb_han},
{"ang",PY_mb_hang},
{"ao",PY_mb_hao},
{"e",PY_mb_he},
{"ei",PY_mb_hei},
{"en",PY_mb_hen},
{"eng",PY_mb_heng},
{"ong",PY_mb_hong},
{"ou",PY_mb_hou},
{"u",PY_mb_hu},
{"ua",PY_mb_hua},
{"uai",PY_mb_huai},
{"uan",PY_mb_huan},
{"uang ",PY_mb_huang},
{"ui",PY_mb_hui},
{"un",PY_mb_hun},
{"uo",PY_mb_huo}};
static struct PY_index  PY_index_i[]={{"",PY_mb_space}};
static struct PY_index  PY_index_j[]={{"i",PY_mb_ji},
{"ia",PY_mb_jia},
{"ian",PY_mb_jian},
{"iang",PY_mb_jiang},
{"iao",PY_mb_jiao},
{"ie",PY_mb_jie},
{"in",PY_mb_jin},
{"ing",PY_mb_jing},
{"iong",PY_mb_jiong},
{"iu",PY_mb_jiu},
{"u",PY_mb_ju},
{"uan",PY_mb_juan},
{"ue",PY_mb_jue},
{"un",PY_mb_jun}};
static struct PY_index  PY_index_k[]={{"a",PY_mb_ka},
{"ai",PY_mb_kai},
{"an",PY_mb_kan},
{"ang",PY_mb_kang},
{"ao",PY_mb_kao},
{"e",PY_mb_ke},
{"en",PY_mb_ken},
{"eng",PY_mb_keng},
{"ong",PY_mb_kong},
{"ou",PY_mb_kou},
{"u",PY_mb_ku},
{"ua",PY_mb_kua},
{"uai",PY_mb_kuai},
{"uan",PY_mb_kuan},
{"uang",PY_mb_kuang},
{"ui",PY_mb_kui},
{"un",PY_mb_kun},
{"uo",PY_mb_kuo}};
static struct PY_index  PY_index_l[]={{"a",PY_mb_la},
{"ai",PY_mb_lai},
{"an",PY_mb_lan},
{"ang",PY_mb_lang},
{"ao",PY_mb_lao},
{"e",PY_mb_le},
{"ei",PY_mb_lei},
{"eng",PY_mb_leng},
{"i",PY_mb_li},
{"ian",PY_mb_lian},
{"iang",PY_mb_liang},
{"iao",PY_mb_liao},
{"ie",PY_mb_lie},
{"in",PY_mb_lin},
{"ing",PY_mb_ling},
{"iu",PY_mb_liu},
{"ong",PY_mb_long},
{"ou",PY_mb_lou},
{"u",PY_mb_lu},
{"uan",PY_mb_luan},
{"ue",PY_mb_lue},
{"un",PY_mb_lun},
{"uo",PY_mb_luo},
{"v",PY_mb_lv}};
static struct PY_index  PY_index_m[]={{"a",PY_mb_ma},
{"ai",PY_mb_mai},
{"an",PY_mb_man},
{"ang",PY_mb_mang},
{"ao",PY_mb_mao},
{"e",PY_mb_me},
{"ei",PY_mb_mei},
{"en",PY_mb_men},
{"eng",PY_mb_meng},
{"i",PY_mb_mi},
{"ian",PY_mb_mian},
{"iao",PY_mb_miao},
{"ie",PY_mb_mie},
{"in",PY_mb_min},
{"ing",PY_mb_ming},
{"iu",PY_mb_miu},
{"o",PY_mb_mo},
{"ou",PY_mb_mou},
{"u",PY_mb_mu}};
static struct PY_index  PY_index_n[]={{"a",PY_mb_na},
{"ai",PY_mb_nai},
{"an",PY_mb_nan},
{"ang",PY_mb_nang},
{"ao",PY_mb_nao},
{"e",PY_mb_ne},
{"ei",PY_mb_nei},
{"en",PY_mb_nen},
{"eng",PY_mb_neng},
{"i",PY_mb_ni},
{"ian",PY_mb_nian},
{"iang",PY_mb_niang},
{"iao",PY_mb_niao},
{"ie",PY_mb_nie},
{"in",PY_mb_nin},
{"ing",PY_mb_ning},
{"iu",PY_mb_niu},
{"ong",PY_mb_nong},
{"u",PY_mb_nu},
{"uan",PY_mb_nuan},
{"ue",PY_mb_nue},
{"uo",PY_mb_nuo},
{"v",PY_mb_nv}};
static struct PY_index  PY_index_o[]={{"",PY_mb_o},
{"u",PY_mb_ou}};
static struct PY_index  PY_index_p[]={{"a",PY_mb_pa},
{"ai",PY_mb_pai},
{"an",PY_mb_pan},
{"ang",PY_mb_pang},
{"ao",PY_mb_pao},
{"ei",PY_mb_pei},
{"en",PY_mb_pen},
{"eng",PY_mb_peng},
{"i",PY_mb_pi},
{"ian",PY_mb_pian},
{"iao",PY_mb_piao},
{"ie",PY_mb_pie},
{"in",PY_mb_pin},
{"ing",PY_mb_ping},
{"o",PY_mb_po},
{"ou",PY_mb_pou},
{"u",PY_mb_pu}};
static struct PY_index  PY_index_q[]={{"i",PY_mb_qi},
{"ia",PY_mb_qia},
{"ian",PY_mb_qian},
{"iang",PY_mb_qiang},
{"iao",PY_mb_qiao},
{"ie",PY_mb_qie},
{"in",PY_mb_qin},
{"ing",PY_mb_qing},
{"iong",PY_mb_qiong},
{"iu",PY_mb_qiu},
{"u",PY_mb_qu},
{"uan",PY_mb_quan},
{"ue",PY_mb_que},
{"un",PY_mb_qun}};
static struct PY_index  PY_index_r[]={{"an",PY_mb_ran},
{"ang",PY_mb_rang},
{"ao",PY_mb_rao},
{"e",PY_mb_re},
{"en",PY_mb_ren},
{"eng",PY_mb_reng},
{"i",PY_mb_ri},
{"ong",PY_mb_rong},
{"ou",PY_mb_rou},
{"u",PY_mb_ru},
{"uan",PY_mb_ruan},
{"ui",PY_mb_rui},
{"un",PY_mb_run},
{"uo",PY_mb_ruo}};
static struct PY_index  PY_index_s[]={{"a",PY_mb_sa},
{"ai",PY_mb_sai},
{"an",PY_mb_san},
{"ang",PY_mb_sang},
{"ao",PY_mb_sao},
{"e",PY_mb_se},
{"en",PY_mb_sen},
{"eng",PY_mb_seng},
{"ha",PY_mb_sha},
{"hai",PY_mb_shai},
{"han",PY_mb_shan},
{"hang ",PY_mb_shang},
{"hao",PY_mb_shao},
{"he",PY_mb_she},
{"hen",PY_mb_shen},
{"heng",PY_mb_sheng},
{"hi",PY_mb_shi},
{"hou",PY_mb_shou},
{"hu",PY_mb_shu},
{"hua",PY_mb_shua},
{"huai",PY_mb_shuai},
{"huan",PY_mb_shuan},
{"huang",PY_mb_shuang},
{"hui",PY_mb_shui},
{"hun",PY_mb_shun},
{"huo",PY_mb_shuo},
{"i",PY_mb_si},
{"ong",PY_mb_song},
{"ou",PY_mb_sou},
{"u",PY_mb_su},
{"uan",PY_mb_suan},
{"ui",PY_mb_sui},
{"un",PY_mb_sun},
{"uo",PY_mb_suo}};
static struct PY_index  PY_index_t[]={{"a",PY_mb_ta},
{"ai",PY_mb_tai},
{"an",PY_mb_tan},
{"ang",PY_mb_tang},
{"ao",PY_mb_tao},
{"e",PY_mb_te},
{"eng",PY_mb_teng},
{"i",PY_mb_ti},
{"ian",PY_mb_tian},
{"iao",PY_mb_tiao},
{"ie",PY_mb_tie},
{"ing",PY_mb_ting},
{"ong",PY_mb_tong},
{"ou",PY_mb_tou},
{"u",PY_mb_tu},
{"uan",PY_mb_tuan},
{"ui",PY_mb_tui},
{"un",PY_mb_tun},
{"uo",PY_mb_tuo}};
static struct PY_index  PY_index_u[]={{"",PY_mb_space}};
static struct PY_index  PY_index_v[]={{"",PY_mb_space}};
static struct PY_index  PY_index_w[]={{"a",PY_mb_wa},
{"ai",PY_mb_wai},
{"an",PY_mb_wan},
{"ang",PY_mb_wang},
{"ei",PY_mb_wei},
{"en",PY_mb_wen},
{"eng",PY_mb_weng},
{"o",PY_mb_wo},
{"u",PY_mb_wu}};
static struct PY_index PY_index_x[]={{"i",PY_mb_xi},
{"ia",PY_mb_xia},
{"ian",PY_mb_xian},
{"iang",PY_mb_xiang},
{"iao",PY_mb_xiao},
{"ie",PY_mb_xie},
{"in",PY_mb_xin},
{"ing",PY_mb_xing},
{"iong",PY_mb_xiong},
{"iu",PY_mb_xiu},
{"u",PY_mb_xu},
{"uan",PY_mb_xuan},
{"ue",PY_mb_xue},
{"un",PY_mb_xun}};
static struct PY_index PY_index_y[]={{"a",PY_mb_ya},
{"an",PY_mb_yan},
{"ang",PY_mb_yang},
{"ao",PY_mb_yao},
{"e",PY_mb_ye},
{"i",PY_mb_yi},
{"in",PY_mb_yin},
{"ing",PY_mb_ying},
{"o",PY_mb_yo},
{"ong",PY_mb_yong},
{"ou",PY_mb_you},
{"u",PY_mb_yu},
{"uan",PY_mb_yuan},
{"ue",PY_mb_yue},
{"un",PY_mb_yun}};
static struct PY_index PY_index_z[]={{"a",PY_mb_za},
{"ai",PY_mb_zai},
{"an",PY_mb_zan},
{"ang",PY_mb_zang},
{"ao",PY_mb_zao},
{"e",PY_mb_ze},
{"ei",PY_mb_zei},
{"en",PY_mb_zen},
{"eng",PY_mb_zeng},
{"ha",PY_mb_zha},
{"hai",PY_mb_zhai},
{"han",PY_mb_zhan},
{"hang",PY_mb_zhang},
{"hao",PY_mb_zhao},
{"he",PY_mb_zhe},
{"hen",PY_mb_zhen},
{"heng",PY_mb_zheng},
{"hi",PY_mb_zhi},
{"hong",PY_mb_zhong},
{"hou",PY_mb_zhou},
{"hu",PY_mb_zhu},
{"hua",PY_mb_zhua},
{"huai",PY_mb_zhuai},
{"huan",PY_mb_zhuan},
{"huang",PY_mb_zhuang},
{"hui",PY_mb_zhui},
{"hun",PY_mb_zhun},
{"huo",PY_mb_zhuo},
{"i",PY_mb_zi},
{"ong",PY_mb_zong},
{"ou",PY_mb_zou},
{"u",PY_mb_zu},
{"uan",PY_mb_zuan},
{"ui",PY_mb_zui},
{"un",PY_mb_zun},
{"uo",PY_mb_zuo}};

static struct PY_index  PY_index_end[]={{"",PY_mb_space}};

static const int PY_index_headsize[]={
	5,
	16,
	33,
	20,
	3,
	9,
	19,
	19,
	0,
	14,
	18,
	24,
	19,
	23,
	2,
	17,
	14,
	14,
	34,
	19,
	0,
	0,
	9,
	14,
	15,
	36
};
/*��������ĸ������*/
static struct PY_index  *PY_index_headletter[]={	PY_index_a,
PY_index_b,
PY_index_c,
PY_index_d,
PY_index_e,
PY_index_f,
PY_index_g,
PY_index_h,
PY_index_i,
PY_index_j,
PY_index_k,
PY_index_l,
PY_index_m,
PY_index_n,
PY_index_o,
PY_index_p,
PY_index_q,
PY_index_r,
PY_index_s,
PY_index_t,
PY_index_u,
PY_index_v,
PY_index_w,
PY_index_x,
PY_index_y,
PY_index_z,
PY_index_end}; 

enum InputStatus
{
	eInputEng = 0,
	eInputChi,
	eSelPY,
	eSelHZ,
	eInputNum
};
class JGBKFont;
class JInputSystem
{
private:
	JGBKFont* mBitmapFont12;
	bool mIsInputActive;

	float mTimer;
	//store input STRING
	char  mInPut[512];
	char * mpInput;
	//store input PY
	char  mPY[16];
	//pointer to the HZ select string.
	char *mHZ;
	//input system status.
	InputStatus mStatus;
	//PY select
	bool mEnablePYSel;
	int mPYShowFirstIndex;
	int mPYSelIndex;
	int mPYSelTableSize;

	//HZ select
	int mHZShowFirstIndex;
	int mHZSelIndex;
	int mHZSelTableSize;
	bool mIsHZ_H;

	bool GetInputKey(int& a, int& b, int& c);
	char* GetChinese(char* str);
	int GetNexPYIndex(char* str, PY_index* &py_index);
	void printf12(char* str,float x, float y, float scale=1.0f, PIXEL_TYPE color=ARGB(255,255,255,255),int type=JGETEXT_LEFT);
	int  strlen12( char* buff, float scale=1.0f);


public:
	static JInputSystem* m_pJInputSystem;
	static JInputSystem * GetInstance();
	static void Destory();
	JInputSystem(void);
	virtual ~JInputSystem(void);

	//Active flag
	void EnableInputMode(char *buf){buf[0]=0;mpInput = buf; mIsInputActive=true;}
	void DisableInputMode(){mpInput = NULL; mIsInputActive=false;}
	//void SetInputActive(bool f){mIsInputActive=f;}
	bool GetIsInputActive(){return mIsInputActive;}

	JGBKFont* GetFont12(){return mBitmapFont12;}

	//Update
	void Update();
	void UpdateInputEng();
	void UpdateInputChi();
	void UpdateSelPY();
	void UpdateSelHZ();
	void UpdateSelHZ_H();
	void UpdateInputNum();

	void Draw();
	void DrawInputString(float x,float y);
	void DrawStr1(char* str, float x, float y, u32 color=ARGB(255,0,0,0));
	void DrawStatus(float x,float y);
	void DrawPYInput(float x,float y);
	void DrawPYSel(float x,float y);
	void DrawHZSel(float x,float y);
	void DrawHZSel_H(float x,float y);
	void DrawInputHelp(float x, float y);

	char * GetInputString(){return mInPut;}
};
#endif

