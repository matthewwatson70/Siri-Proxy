// Copyright (c) 2011,cd-team.org.
//
// Permission is hereby granted, free of charge, to any person or organization
// obtaining a copy of the software and accompanying documentation covered by
// this license (the "Software") to use, reproduce, display, distribute,
// execute, and transmit the Software, and to prepare derivative works of the
// Software, and to permit third-parties to whom the Software is furnished to
// do so, all subject to the following:
// 
// The copyright notices in the Software and this entire statement, including
// the above license grant, this restriction and the following disclaimer,
// must be included in all copies of the Software, in whole or in part, and
// all derivative works of the Software, unless such copies or derivative
// works are solely in the form of machine-executable object code generated by
// a source language processor.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
// SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
// FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.
//
#include "HtmlHelper.h"
#include "SiriTcpSvr.h"
#include "DbHelpSQL.h"
#include "SiriTokenProvider.h"
#include <Poco/Format.h>
#include <Poco/NumberFormatter.h>
#include <Poco/DateTimeFormatter.h>
#include <Poco/Util/Application.h>
const string DATE_FORMAT_STR="%Y-%n-%d %H-%M-%S";
using namespace Poco;
using namespace Poco::Net;
using namespace Poco::Util;

const string HtmlHelper::_content_hold="{F49D619B-CD8B-4e72-BC3A-5E61CEF5DDA0}";
const string HtmlHelper::_title_hold="{660B959D-1422-4e23-86EA-396A74D951EA}";
const string HtmlHelper::_link_keylist_hold="{86789C3D-4F99-4403-ADFF-278D4BFE071A}";
const string HtmlHelper::_link_trace_hold="{A2B2C418-5C62-48dc-8DDA-5598AC064C61}";

string HtmlHelper::admin_index_url;
string HtmlHelper::admin_base_url;
string HtmlHelper::key_man_url;
string HtmlHelper::key_trace_url;


extern bool g_enable_iphone4_server;

#pragma region //主页
const string HtmlHelper::_html_index_tmpl="<html>\
<head>\
<title>服务器运行信息</title>\
<meta http-equiv=\"refresh\" content=\"5\">\
<meta http-equiv=\"Content-Type\" content=\"text/html;charset=gb2312\">\
<style type=\"text/css\">\
.intro-main { padding: 20px; line-height: 24px;  }\
.intro-main { width:1024px; overflow:hidden; }\
.intro-main-b td { border-top: 1px solid #CCC; border-right: 1px none #CCC; border-bottom: 1px none #CCC; border-left: 1px solid #CCC; padding:4px; }\
.intro-main-b .specification .desc li { width:550px; }\
.intro-main-b .specification .desc { width:650px; }\
.intro-main-b table { border-top: 1px none #CCC; border-right: 1px solid #CCC; border-bottom: 1px solid #CCC; border-left: 1px none #CCC; }\
.intro-main-c td { border-top: 1px solid #CCC; border-right: 1px none #CCC; border-bottom: 1px none #CCC; border-left: 1px solid #CCC; padding:4px; }\
.intro-main-c table { border-top: 1px none #CCC; border-right: 1px solid #CCC; border-bottom: 1px solid #CCC; border-left: 1px none #CCC; }\
.intro-main a { width:300px; }\
.intro-main ul { width:auto; height:auto; background:none; }\
.intro-main li { width:723px; height:auto; line-height:24px; display:block; }\
.intro-main p { width:auto; height:auto; line-height:24px; }\
.intro-main .intro-tips { margin-bottom: 10px; }\
.intro-main-b table .title{background:#f0f0f0;font-weight:bold;}\
.intro-main-b table .name{width:300px;}\
.intro-main-b table .redclr{color:RGB(255,0,0)}\
</style>\
</head>\
<body style=\"text-align:center\">\
<div class='intro-main intro-main-b'>\
  <a href='{86789C3D-4F99-4403-ADFF-278D4BFE071A}' style='float:right'>密钥列表</a>\
  <a href='{A2B2C418-5C62-48dc-8DDA-5598AC064C61}' style='float:right'>客户端访问跟踪</a>\
</div>\
<div id=\"c_parameters\" class=\"intro-main intro-main-b\">\
<table class=\"specification\" cellspacing=\"0\" cellpadding=\"0\">\
<tbody>\
{F49D619B-CD8B-4e72-BC3A-5E61CEF5DDA0}\
</tbody>\
</table>\
</div>\
</body>\
</html>";
#pragma endregion
#pragma region //key过期时间,key使用信息

const string HtmlHelper::_html_tmpl="<html>\
	<head>\
	<title>{660B959D-1422-4e23-86EA-396A74D951EA}</title>\
	<meta http-equiv=\"Content-Type\" content=\"text/html;charset=gb2312\">\
	<style type=\"text/css\">\
	.intro-main { padding: 20px; line-height: 24px;  }\
	.intro-main { width:1024px; overflow:hidden; }\
	.intro-main-b td { border-top: 1px solid #CCC; border-right: 1px none #CCC; border-bottom: 1px none #CCC; border-left: 1px solid #CCC; padding:4px; }\
	.intro-main-b .specification .desc li { width:550px; }\
	.intro-main-b .specification .desc { width:650px; }\
	.intro-main-b table { border-top: 1px none #CCC; border-right: 1px solid #CCC; border-bottom: 1px solid #CCC; border-left: 1px none #CCC; }\
	.intro-main-c td { border-top: 1px solid #CCC; border-right: 1px none #CCC; border-bottom: 1px none #CCC; border-left: 1px solid #CCC; padding:4px; }\
	.intro-main-c table { border-top: 1px none #CCC; border-right: 1px solid #CCC; border-bottom: 1px solid #CCC; border-left: 1px none #CCC; }\
	.intro-main a { width:300px; }\
	.intro-main ul { width:auto; height:auto; background:none; }\
	.intro-main li { width:723px; height:auto; line-height:24px; display:block; }\
	.intro-main p { width:auto; height:auto; line-height:24px; }\
	.intro-main .intro-tips { margin-bottom: 10px; }\
	.intro-main-b table .title{background:#f0f0f0;font-weight:bold;}\
	.intro-main-b table .name{width:300px;}\
	.intro-main-b table .redclr{color:RGB(255,0,0)}\
	</style>\
	</head>\
	<body style=\"text-align:center\">\
	<div id=\"c_parameters\" class=\"intro-main intro-main-b\">\
	<table class=\"specification\" cellspacing=\"0\" cellpadding=\"0\">\
	<tbody>\
	{F49D619B-CD8B-4e72-BC3A-5E61CEF5DDA0}\
	</tbody>\
	</table>\
	</div>\
	</body>\
	</html>";
#pragma endregion

void HtmlHelper::Print4sServerInfo(stringstream& ss)
{
	SiriTcpSvr* iphone4s=SiriTcpSvr::iPhone4sServer();
	size_t con2=iphone4s->currentConnections();
	size_t n2=iphone4s->currentThreads();
	ss<<MakeTableTitleHtml("iPhone4s服务器信息");
	ss<<MakeTableTwoColumnRow("启动时间戳(北京时间)",format_time_t_string(iphone4s->startTime));
	ss<<MakeTableTwoColumnRow("证书颁发给",iphone4s->pX509Certificate->commonName());
	ss<<MakeTableTwoColumnRow("证书颁发者",iphone4s->pX509Certificate->issuerName());
	ss<<MakeTableTwoColumnRow("证书主题",iphone4s->pX509Certificate->subjectName());
	ss<<MakeTableTwoColumnRow("最大线程数",NumberFormatter::format(iphone4s->params().getMaxThreads()));
	ss<<MakeTableTwoColumnRow("最大连接队列",NumberFormatter::format(iphone4s->params().getMaxQueued()));
	ss<<MakeTableTwoColumnRow("客户端连接数",NumberFormatter::format(con2));
	ss<<MakeTableTwoColumnRow("活动线程数",NumberFormatter::format(n2));
}

void HtmlHelper::Print4ServerInfo(stringstream& ss)
{
	SiriTcpSvr* iphone4=SiriTcpSvr::iPhone4Server();
	size_t con1=iphone4->currentConnections();
	size_t n1=iphone4->currentThreads();
	size_t maxn1=iphone4->maxConcurrentConnections();
	size_t total1=iphone4->totalConnections();
	size_t q1=iphone4->queuedConnections();
	size_t r1=iphone4->refusedConnections();

	ss<<MakeTableTitleHtml("iPhone4服务器信息");
	ss<<MakeTableTwoColumnRow("启动时间戳(北京时间)",format_time_t_string(iphone4->startTime));
	ss<<MakeTableTwoColumnRow("证书颁发给",iphone4->pX509Certificate->commonName());
	ss<<MakeTableTwoColumnRow("证书颁发者",iphone4->pX509Certificate->issuerName());
	ss<<MakeTableTwoColumnRow("证书主题",iphone4->pX509Certificate->subjectName());
	ss<<MakeTableTwoColumnRow("最大线程数",NumberFormatter::format(iphone4->params().getMaxThreads()));
	ss<<MakeTableTwoColumnRow("最大连接队列",NumberFormatter::format(iphone4->params().getMaxQueued()));
	ss<<MakeTableTwoColumnRow("客户端连接数",NumberFormatter::format(con1));
	ss<<MakeTableTwoColumnRow("活动线程数",NumberFormatter::format(n1));
	ss<<MakeTableTwoColumnRow("最高并发连接数",NumberFormatter::format(maxn1));
	ss<<MakeTableTwoColumnRow("已处理连接总数",NumberFormatter::format(total1));
	ss<<MakeTableTwoColumnRow("队列等候连接数",NumberFormatter::format(q1));
	ss<<MakeTableTwoColumnRow("已拒绝的连接数",NumberFormatter::format(r1));
}

void HtmlHelper::PrintHostInfo( stringstream& ss )
{
	Print4sServerInfo(ss);
	if(g_enable_iphone4_server)
	{
		Print4sServerInfo(ss);
	}
}

void HtmlHelper::PrintAllTickets(stringstream& ss)
{
	ss<<MakeTableTitleHtml("密钥信息");
	ss<<MakeTableTwoColumnRow("数据库密钥数",NumberFormatter::format(SiriTokenProvider::GetTotalTicketCount()));
	ss<<MakeTableTwoColumnRow("已过期密钥数",NumberFormatter::format(SiriTokenProvider::GetExpiredTicketCount()));
	ss<<MakeTableTwoColumnRow("缓存未过期密钥数",NumberFormatter::format(SiriTokenProvider::GetCacheTicketCount()));
	ss<<MakeTableTwoColumnRow("正在使用的密钥数",NumberFormatter::format(SiriTokenProvider::GetTickeInUseCount()));
}

string HtmlHelper::MakeTableTitleHtml( const string& title )
{
	return Poco::format("<tr><td class=\"title\" colspan=\"2\">%s</td></tr>",title);
}

std::string HtmlHelper::MakeTableTwoColumnRow( const string& name,const string& desc )
{
	return Poco::format("<tr><td class=\"name\">%s</td><td class=\"desc\">%s</td></tr>",name,desc);
}

std::string HtmlHelper::MakeTableTwoTitleRow( const string& s1,const string& s2 )
{
	return Poco::format("<tr><td class=\"name title\">%s</td><td class=\"desc title\">%s</td></tr>",s1,s2);
}

std::string HtmlHelper::MakeTableThreeColumnRow( const string& s1,const string& s2,const string& s3 )
{
	return Poco::format("<tr><td class=\"desc\">%s</td><td class=\"desc\">%s</td><td class=\"name\">%s</td></tr>",s1,s2,s3);
}

std::string HtmlHelper::MakeTableThreeTitleRow( const string& s1,const string& s2,const string& s3 )
{
	return Poco::format("<tr><td class=\"name title\">%s</td><td class=\"desc title\">%s</td><td class=\"desc title\">%s</td></tr>",s1,s2,s3);
}

std::string HtmlHelper::GetHostHtmlInfo()
{
	stringstream ss;
	PrintHostInfo(ss);
	PrintAllTickets(ss);
	string content=ss.str();
	string html= Poco::replace(_html_index_tmpl,_content_hold,content);
	html=replace(html,_link_keylist_hold,key_man_url);
	html=replace(html,_link_trace_hold,key_trace_url);
	return html;
}

void HtmlHelper::InitUrls()
{
	LayeredConfiguration& cfg=Application::instance().config();
	string encrypt =cfg.getString("HttpSvr.admin.encrypt");
	encrypt=Poco::toLower(encrypt);
	admin_base_url=format("/admin/%s/",encrypt);
	admin_index_url=admin_base_url+"index.html";
	key_man_url=admin_base_url+"keylist.html";
	key_trace_url=admin_base_url+"key_trace.html";
}

std::string HtmlHelper::GetTraceHtml()
{
	stringstream ss;
	vector<key_acl_log> logs= DbHelpSQL::Current()->GetKeyAclDetail();
	ss<<MakeTableThreeTitleRow(format("KEY(%d)",(int)logs.size()),"访问次数","IP地址计数(不重复)");
	for (vector<key_acl_log>::iterator itr=logs.begin();itr!=logs.end();itr++)
	{
		ss<<MakeTableThreeColumnRow(itr->key_id,NumberFormatter::format(itr->hints),NumberFormatter::format(itr->ip_count));
	}
	string html=ss.str();
	html=replace(_html_tmpl,_content_hold,html);
	html=replace(html,_title_hold,string("KEY使用情况跟踪"));
	return html;
}

std::string HtmlHelper::GetKeyListHtml()
{
	stringstream ss;
	
	ss<<MakeTableThreeTitleRow("AssistantId",format("过期计数(WaterMark:%d)",Ticket::EXPIRE_WATER_MARK),"最后更新日期(北京时间)");
	vector<Ticket> tickets= DbHelpSQL::Current()->GetAllTickets();
	for (vector<Ticket>::iterator itr=tickets.begin();itr!=tickets.end();itr++)
	{
		string str_expired=NumberFormatter::format(itr->expired);
		if(itr->IsExpired())
		{
			str_expired=format("<span class='redclr'>%s(已过期)</span>",str_expired);
		}
		ss<<MakeTableThreeColumnRow(itr->assistantId,str_expired,format_time_t_string(itr->modify));
	}
	string html=ss.str();
	html=replace(_html_tmpl,_content_hold,html);
	html=replace(html,_title_hold,string("服务器所有密钥列表"));
	return html;
}
