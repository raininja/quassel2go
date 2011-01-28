import Qt 4.7

ListModel {
    id: chatModel

    ListElement {
        plaintext: "Apple"
    }
    ListElement {
        plaintext: "Orange"
    }
    ListElement {
        plaintext: "Banana"
    }

    ListElement { plaintext: "[16:06:04] <[mat^2]> hey. I want to make QGraphicsPixmapItem's in a QGraphicsScene resizable (the built-in move and selection stuff works perfectly already). any directions on how to do this?" }
    ListElement { plaintext: "[16:06:07] --> Triskelios (~trisk@24-181-227-22.dhcp.oxfr.ma.charter.com) hat #qt betreten" }
    ListElement { plaintext: "[16:06:10] <xlinkz0> it sends function calls ofer TCP all over the place?" }
    ListElement { plaintext: "[16:06:12] <thirtySeven> cbreak: http://pastebin.com/0PRwYgT0" }
    ListElement { plaintext: "[16:06:21] <a_l_e> and addin ghe filename to the parameters was also a good idea..." }
    ListElement { plaintext: "[16:06:23] <richmoore3> cbreak: there's no method to perform connect via the metaobject" }
    ListElement { plaintext: "[16:06:32] <peppe> [mat^2]: provide handles for resizing, and implement resizing :)" }
    ListElement { plaintext: "[16:06:38] <cbreak> richmoore1: you can use connect, with some trickery" }
    ListElement { plaintext: "[16:06:39] <xlinkz0> and using identifier bytes you use much less network traffic" }
    ListElement { plaintext: "[16:06:57] <[mat^2]> peppe: hehe ;) I just wouldn't want to reinvent the wheel if someone had code that did this already" }
    ListElement { plaintext: "[16:06:58] <cbreak> richmoore3: or you can invoke if you only want to call directly" }
    ListElement { plaintext: "[16:07:01] <peppe> the only trickery is that you MUST prepend the \"2\" and the \"1\"" }
    ListElement { plaintext: "[16:07:05] <pumphaus> Fazer2_: this http://codepad.org/xeWawwTo" }
    ListElement { plaintext: "[16:07:32] <mal|lappy> thirtySeven: You're never calling AppServer::attach" }
    ListElement { plaintext: "[16:07:34] <richmoore3> cbreak: yes, i know - eg. in kjsembed i was dynamically accessing all this stuff in order to produce js bindings to qobjects on the fly :-)" }
    ListElement { plaintext: "[16:07:35] <xlinkz0> why would you help people develop bad practices?" }
    ListElement { plaintext: "[16:07:35] <cbreak> thirtySeven: and where's the problem?" }
    ListElement { plaintext: "[16:07:38] <mal|lappy> to set the view inside appserver to the correct pointer" }
    ListElement { plaintext: "[16:07:56] <Fazer2_> pumphaus: I don't want that way, because the user of the function has to remember to add \"SLOT(...)\"" }
    ListElement { plaintext: "[16:08:05] <pumphaus> Fazer2_: well, yes" }
    ListElement { plaintext: "[16:08:10] <pumphaus> Fazer2_: you have to in Qt, too" }
    ListElement { plaintext: "[16:08:21] <pumphaus> Fazer2_: just look at QTimer" }
    ListElement { plaintext: "[16:08:46] --> akem (~User_name@unaffiliated/akem) hat #qt betreten" }
    ListElement { plaintext: "[16:08:50] <pumphaus> otherwise, the user might also want to connect to a SIGNAL instead of a SLOT" }
    ListElement { plaintext: "[16:08:51] <richmoore3> Fazer2_: it will work if you prepend the 1, it's just safer to use the macros" }
    ListElement { plaintext: "[16:08:52] <thirtySeven> mal|lappy: dude.. that was fast" }
    ListElement { plaintext: "[16:08:57] <thirtySeven> and yes.. it solved my problem" }
    ListElement { plaintext: "[16:09:01] <mal|lappy> Good" }
    ListElement { plaintext: "[16:09:02] <cbreak> thirtySeven: are you aware that you never exec() the QApplication?" }
    ListElement { plaintext: "[16:09:09] <thirtySeven> thanks a lot lappy" }
    ListElement { plaintext: "[16:09:11] <thirtySeven> :)" }
    ListElement { plaintext: "[16:09:15] <Fazer2_> richmoore3: hm you're right" }
    ListElement { plaintext: "[16:09:17] <thirtySeven> cbreak: umm.. nope I wasnt?" }
    ListElement { plaintext: "[16:09:24] <thirtySeven> cbreak: but why?" }
    ListElement { plaintext: "[16:09:35] <cbreak> you need that for an event loop" }
    ListElement { plaintext: "[16:09:55] <thirtySeven> cbreak: I didn't even write it. It's just from the generated code of Qt Creator" }
    ListElement { plaintext: "[16:10:06] <LinuxUser> http://codepad.org/5Joapvv2 could anybody tell me why http->readAll() doesn't output anything? the \"Test\" output is done and url is right too :(" }
    ListElement { plaintext: "[16:10:06] <thirtySeven> but I have a TCP server.. so won't I need an event loop?" }
    ListElement { plaintext: "[16:10:17] <cbreak> of course you need an event loop" }
    ListElement { plaintext: "[16:10:24] <cbreak> how do you think signals and slots are dispatched?" }
    ListElement { plaintext: "[16:10:24] <thirtySeven> and there is a QGraphicsView that will also be there.. I mean there is a GUI" }
    ListElement { plaintext: "[16:10:33] <pumphaus> Fazer2_: example of how Qt does this: http://doc.qt.nokia.com/latest/qtimer.html#singleShot - you need to pass 'member' with SIGNAL or SLOT as well" }
    ListElement { plaintext: "[16:10:39] <thirtySeven> exactly.. so won't I need to call a.exec?" }
    ListElement { plaintext: "[16:10:40] <cbreak> and hwo do you think the GUI is updated?" }
    ListElement { plaintext: "[16:10:53] <cbreak> without an event loop, all that won't work" }
    ListElement { plaintext: "[16:10:59] <cbreak> no signals, no slots, no GUI updates" }
    ListElement { plaintext: "[16:11:02] <cbreak> no mouse clicks" }
    ListElement { plaintext: "[16:11:14] <thirtySeven> umm.. my point exactly" }
    ListElement { plaintext: "[16:11:23] <thirtySeven> so.. I would need to call a.exec() right??" }
    ListElement { plaintext: "[16:11:26] <richmoore3> LinuxUser: one thing to remember is QHttp is deprecated. you should use QNetworkAccessManager for new code" }
    ListElement { plaintext: "[16:11:39] <Fazer2_> pumphaus: alright, thanks! :-)" }
    ListElement { plaintext: "[16:12:00] <thirtySeven> oh.. I get the point.. actually the last line of my main() (which I trimmed by mistake) was: return a.exec();" }
    ListElement { plaintext: "[16:12:18] <LinuxUser> richmoore3: ok, thx, then i will take a look at that" }
    ListElement { plaintext: "[16:12:31] <thirtySeven> in the last part of main, I actually do a lot of calculations and stuff so it's almost 200+ lines so I removed it.. and removed a.exec() by mistake" }
    ListElement { plaintext: "[16:12:42] <cbreak> also, as I expected" }
    ListElement { plaintext: "[16:12:50] <cbreak> you seem to create a HyperSprite in main" }
    ListElement { plaintext: "[16:12:52] <cbreak> but not use it" }
    ListElement { plaintext: "[16:12:56] --> __name__ (~name@chello062178002083.1.11.vie.surfer.at) hat #qt betreten" }
    ListElement { plaintext: "[16:12:56] <-- __name__ (~name@chello062178002083.1.11.vie.surfer.at) hat das Netzwerk verlassen (Changing host)" }
    ListElement { plaintext: "[16:12:56] --> __name__ (~name@sburn/devel/name) hat #qt betreten" }
    ListElement { plaintext: "[16:13:01] <-- wbooze` (~levgue@xdsl-84-44-211-203.netcologne.de) hat das Netzwerk verlassen (Quit: ERC Version 5.3 (IRC client for Emacs))" }
    ListElement { plaintext: "[16:13:28] <thirtySeven> cbreak: and you were right: If I'm asking for help, I am not the one to judge what code is important and what is not :)" }
    ListElement { plaintext: "[16:13:34] <cbreak> and the HyperSprite pointer in AppServer never gets filled with anything useful" }
    ListElement { plaintext: "[16:13:37] <thirtySeven> actually.. the AppServer::attach takes care of it" }
    ListElement { plaintext: "[16:13:56] <cbreak> you don't call that though" }
    ListElement { plaintext: "[16:14:06] <thirtySeven> which was the error in my code" }
    ListElement { plaintext: "[16:14:12] <thirtySeven> which lead to the segmentation fault" }
    ListElement { plaintext: "[16:14:25] <thirtySeven> i mean.. it is supposed to be there.. and now it is" }
    ListElement { plaintext: "[16:14:38] <cbreak> a definite improvement." }
    ListElement { plaintext: "[16:14:50] <thirtySeven> cbreak: quite a serious person you are, aren't you?" }
    ListElement { plaintext: "[16:14:58] <cbreak> yes." }
    ListElement { plaintext: "[16:15:02] <cbreak> I like to pretend to be a robot." }
    ListElement { plaintext: "[16:15:06] <thirtySeven> :)" }
    ListElement { plaintext: "[16:15:24] <-- guardian (~guardian@i07v-62-34-48-83.d4.club-internet.fr) hat das Netzwerk verlassen (Quit: Leaving.)" }
    ListElement { plaintext: "[16:15:29] <thirtySeven> cbreak: either ways, I am very very grateful" }
    ListElement { plaintext: "[16:15:36] <thirtySeven> cbreak: and amazed" }
    ListElement { plaintext: "[16:15:55] <thirtySeven> cbreak: amazed by your \"pointing-out-mistakes/minute rating\"" }
    ListElement { plaintext: "[16:16:12] <thirtySeven> cbreak: cant believe I spent an afternoon over this issue" }
    ListElement { plaintext: "[16:16:30] <cbreak> I probably spent more time than you on sillier mistakes :/" }
    ListElement { plaintext: "[16:16:43] <thirtySeven> cbreak: ahhh.. the life of a coder" }
    ListElement { plaintext: "[16:16:56] <cbreak> I lost days to a missing bracket in some branch condition" }
    ListElement { plaintext: "[16:17:12] <thirtySeven> oh.. I know that one" }
    ListElement { plaintext: "[16:17:26] --> JakeSays (~quassel@c-98-202-184-139.hsd1.ut.comcast.net) hat #qt betreten" }
    ListElement { plaintext: "[16:17:26] *** Modus #qt +v JakeSays durch ChanServ" }
    ListElement { plaintext: "[16:17:32] <thirtySeven> and the worst being that it is syntatically correct and no one to tell you where something went wrong" }
    ListElement { plaintext: "[16:17:45] <thirtySeven> anywho.. another question I had.. why is it sometimes that std::cout doesn't output anything to a terminal but qDebug does?" }
    ListElement { plaintext: "[16:18:07] <cbreak> I mean, who made & be lower binding strenth than ==? that's just evil" }
    ListElement { plaintext: "[16:18:26] <thirtySeven> I mean i was writing some debugging output.. and any std::cout statement didn't output anything. I changed it qDebug and it worked" }
    ListElement { plaintext: "[16:18:26] <cbreak> thirtySeven: windows?" }
    ListElement { plaintext: "[16:18:34] <thirtySeven> umm.. linux" }
    ListElement { plaintext: "[16:18:37] <Fazer2_> bye" }
    ListElement { plaintext: "[16:18:37] <cbreak> hmm." }
    ListElement { plaintext: "[16:18:40] <-- Fazer2_ (~Fazer2@aeac2.neoplus.adsl.tpnet.pl) hat das Netzwerk verlassen (Remote host closed the connection)" }
    ListElement { plaintext: "[16:18:43] <thirtySeven> don't know the behaviour on windows.. never developed on windows" }
    ListElement { plaintext: "[16:18:55] <-- mbnoimi (~quassel@178.171.201.67) hat das Netzwerk verlassen (Quit: http://quassel-irc.org - Chat comfortably. Anywhere.)" }
    ListElement { plaintext: "[16:18:57] <cbreak> windows doesn't attach stdout and stderr I think" }
    ListElement { plaintext: "[16:19:03] <peppe> do you flush cout?" }
    ListElement { plaintext: "[16:19:15] <cbreak> << std::endl; should flush" }
    ListElement { plaintext: "[16:20:02] --> BiggAl (~user@5ad5d056.bb.sky.com) hat #qt betreten" }
    ListElement { plaintext: "[16:20:20] <thirtySeven> cbreak: always used std::endl at end" }
    ListElement { plaintext: "[16:20:31] <thirtySeven> peppe: i.e. always flushed" }
    ListElement { plaintext: "[16:20:45] --> mbnoimi_ (~quassel@178.171.201.67) hat #qt betreten" }
    ListElement { plaintext: "[16:22:49] --> thezeroth (~abhishek@115.184.81.144) hat #qt betreten" }
    ListElement { plaintext: "[16:24:01] <thirtySeven> allright.. just confirmed.. the ones I flushed did work and a few lines where I did not use it.. didnt" }
    ListElement { plaintext: "[16:24:16] <thirtySeven> I guess I dont really have any respeect in this channel anymore, now do I :(" }
    ListElement { plaintext: "[16:24:53] <JakeSays> thirtySeven: why would you?" }
    ListElement { plaintext: "[16:26:17] <domme> do i really have to subclass qcombobox to make something happen when it gets focus?" }
    ListElement { plaintext: "[16:27:01] --> h32Lg (~IdentX@mnch-5d85781d.pool.mediaWays.net) hat #qt betreten" }
    ListElement { plaintext: "[16:27:01] <-- h32Lg (~IdentX@mnch-5d85781d.pool.mediaWays.net) hat das Netzwerk verlassen (Changing host)" }
    ListElement { plaintext: "[16:27:02] --> h32Lg (~IdentX@unaffiliated/h32lg) hat #qt betreten" }
    ListElement { plaintext: "[16:27:07] <peppe> domme: or event filter" }
    ListElement { plaintext: "[16:27:19] <-- earthnail (~earthnail@p50895E12.dip.t-dialin.net) hat das Netzwerk verlassen (Quit: earthnail)" }
    ListElement { plaintext: "[16:27:56] --> kszonek (~kszonek@abau79.neoplus.adsl.tpnet.pl) hat #qt betreten" }
    ListElement { plaintext: "[16:28:02] <-- MrFahrenheit (~RageOfTho@users-146-124.vinet.ba) hat das Netzwerk verlassen (Read error: Connection reset by peer)" }
    ListElement { plaintext: "[16:28:27] --> MrFahrenheit (~RageOfTho@users-146-124.vinet.ba) hat #qt betreten" }
    ListElement { plaintext: "[16:28:55] --> trichard_ (~quassel@150.72-200-80.adsl-dyn.isp.belgacom.be) hat #qt betreten" }
    ListElement { plaintext: "[16:29:07] <-- mbnoimi_ (~quassel@178.171.201.67) hat #qt verlassen (\"http://quassel-irc.org - Chat comfortably. Anywhere.\")" }
    ListElement { plaintext: "[16:19:28] <peppe> that's what I'm asking" }
}
