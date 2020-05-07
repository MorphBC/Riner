

#include "DagFile.h"
#include <src/util/Logging.h>
#include <thread>
#include <src/compute/DeviceId.h>
#include <src/common/PlatformDefines.h>


static const uint32_t dagSize[2048] = {
    8388593U, 8454143U, 8519647U, 8585209U, 8650727U, 8716283U, 8781797U, 8847359U,
    8912887U, 8978381U, 9043967U, 9109483U, 9175037U, 9240523U, 9306097U, 9371629U,
    9437179U, 9502697U, 9568219U, 9633787U, 9699323U, 9764863U, 9830393U, 9895889U,
    9961463U, 10026979U, 10092539U, 10158079U, 10223593U, 10289141U, 10354667U, 10420217U,
    10485751U, 10551263U, 10616831U, 10682339U, 10747903U, 10813423U, 10878961U, 10944511U,
    11010037U, 11075573U, 11141113U, 11206649U, 11272181U, 11337727U, 11403247U, 11468797U,
    11534329U, 11599867U, 11665403U, 11730931U, 11796469U, 11862013U, 11927551U, 11993083U,
    12058621U, 12124157U, 12189677U, 12255227U, 12320753U, 12386299U, 12451807U, 12517339U,
    12582893U, 12648439U, 12713959U, 12779483U, 12845033U, 12910591U, 12976121U, 13041661U,
    13107197U, 13172729U, 13238263U, 13303799U, 13369333U, 13434853U, 13500373U, 13565897U,
    13631477U, 13697023U, 13762549U, 13828093U, 13893613U, 13959163U, 14024671U, 14090239U,
    14155763U, 14221309U, 14286809U, 14352367U, 14417881U, 14483437U, 14548979U, 14614507U,
    14680063U, 14745559U, 14811133U, 14876657U, 14942197U, 15007723U, 15073277U, 15138793U,
    15204349U, 15269869U, 15335407U, 15400951U, 15466463U, 15531977U, 15597559U, 15663083U,
    15728611U, 15794171U, 15859687U, 15925241U, 15990781U, 16056317U, 16121849U, 16187359U,
    16252919U, 16318459U, 16383977U, 16449529U, 16515067U, 16580587U, 16646099U, 16711661U,
    16777213U, 16842751U, 16908263U, 16973767U, 17039339U, 17104891U, 17170429U, 17235961U,
    17301463U, 17367029U, 17432561U, 17498111U, 17563633U, 17629123U, 17694709U, 17760251U,
    17825791U, 17891299U, 17956849U, 18022399U, 18087899U, 18153431U, 18219001U, 18284533U,
    18350063U, 18415597U, 18481097U, 18546617U, 18612211U, 18677723U, 18743281U, 18808831U,
    18874367U, 18939901U, 19005433U, 19070971U, 19136503U, 19202041U, 19267561U, 19333103U,
    19398647U, 19464173U, 19529717U, 19595249U, 19660799U, 19726331U, 19791869U, 19857371U,
    19922923U, 19988477U, 20054011U, 20119543U, 20185051U, 20250577U, 20316151U, 20381689U,
    20447191U, 20512747U, 20578297U, 20643809U, 20709347U, 20774909U, 20840429U, 20905979U,
    20971507U, 21037021U, 21102583U, 21168113U, 21233651U, 21299167U, 21364727U, 21430259U,
    21495797U, 21561341U, 21626819U, 21692387U, 21757951U, 21823453U, 21889019U, 21954547U,
    22020091U, 22085621U, 22151167U, 22216673U, 22282199U, 22347769U, 22413289U, 22478803U,
    22544351U, 22609919U, 22675403U, 22740961U, 22806521U, 22872053U, 22937591U, 23003131U,
    23068667U, 23134201U, 23199731U, 23265247U, 23330773U, 23396339U, 23461877U, 23527409U,
    23592937U, 23658493U, 23724031U, 23789561U, 23855101U, 23920607U, 23986159U, 24051683U,
    24117217U, 24182773U, 24248299U, 24313853U, 24379391U, 24444881U, 24510463U, 24575977U,
    24641479U, 24707071U, 24772603U, 24838127U, 24903667U, 24969193U, 25034731U, 25100287U,
    25165813U, 25231351U, 25296893U, 25362431U, 25427957U, 25493483U, 25559033U, 25624567U,
    25690097U, 25755619U, 25821179U, 25886719U, 25952243U, 26017759U, 26083273U, 26148853U,
    26214379U, 26279899U, 26345471U, 26411003U, 26476543U, 26542067U, 26607611U, 26673149U,
    26738687U, 26804203U, 26869753U, 26935267U, 27000817U, 27066359U, 27131903U, 27197413U,
    27262931U, 27328507U, 27394019U, 27459581U, 27525109U, 27590653U, 27656149U, 27721721U,
    27787213U, 27852791U, 27918323U, 27983863U, 28049407U, 28114943U, 28180459U, 28246003U,
    28311541U, 28377077U, 28442551U, 28508153U, 28573673U, 28639231U, 28704749U, 28770293U,
    28835819U, 28901311U, 28966909U, 29032441U, 29097977U, 29163481U, 29229047U, 29294581U,
    29360087U, 29425637U, 29491193U, 29556719U, 29622269U, 29687783U, 29753341U, 29818871U,
    29884411U, 29949943U, 30015481U, 30081019U, 30146531U, 30212093U, 30277627U, 30343129U,
    30408701U, 30474239U, 30539749U, 30605303U, 30670847U, 30736379U, 30801917U, 30867439U,
    30932987U, 30998509U, 31064063U, 31129597U, 31195117U, 31260653U, 31326181U, 31391729U,
    31457269U, 31522747U, 31588351U, 31653871U, 31719409U, 31784941U, 31850491U, 31916011U,
    31981567U, 32047097U, 32112607U, 32178169U, 32243707U, 32309243U, 32374781U, 32440319U,
    32505829U, 32571373U, 32636921U, 32702443U, 32767997U, 32833531U, 32899037U, 32964559U,
    33030121U, 33095677U, 33161201U, 33226741U, 33292283U, 33357811U, 33423319U, 33488891U,
    33554393U, 33619919U, 33685493U, 33751019U, 33816571U, 33882103U, 33947621U, 34013183U,
    34078699U, 34144207U, 34209787U, 34275301U, 34340861U, 34406399U, 34471933U, 34537427U,
    34602991U, 34668527U, 34734079U, 34799563U, 34865141U, 34930619U, 34996223U, 35061751U,
    35127263U, 35192831U, 35258347U, 35323903U, 35389423U, 35454943U, 35520467U, 35586017U,
    35651579U, 35717111U, 35782613U, 35848187U, 35913727U, 35979257U, 36044797U, 36110311U,
    36175871U, 36241397U, 36306937U, 36372463U, 36438013U, 36503531U, 36569083U, 36634621U,
    36700159U, 36765683U, 36831227U, 36896767U, 36962291U, 37027831U, 37093373U, 37158911U,
    37224437U, 37289957U, 37355503U, 37421053U, 37486591U, 37552091U, 37617653U, 37683199U,
    37748717U, 37814267U, 37879783U, 37945339U, 38010871U, 38076407U, 38141951U, 38207479U,
    38273023U, 38338541U, 38404081U, 38469617U, 38535151U, 38600701U, 38666219U, 38731769U,
    38797303U, 38862797U, 38928371U, 38993917U, 39059431U, 39124991U, 39190519U, 39255967U,
    39321599U, 39387067U, 39452671U, 39518201U, 39583727U, 39649277U, 39714799U, 39780343U,
    39845887U, 39911423U, 39976939U, 40042469U, 40108027U, 40173557U, 40239103U, 40304633U,
    40370173U, 40435699U, 40501231U, 40566749U, 40632313U, 40697851U, 40763369U, 40828927U,
    40894457U, 40959979U, 41025499U, 41091047U, 41156569U, 41222143U, 41287651U, 41353201U,
    41418739U, 41484271U, 41549803U, 41615347U, 41680871U, 41746423U, 41811949U, 41877499U,
    41943023U, 42008567U, 42074101U, 42139619U, 42205183U, 42270707U, 42336253U, 42401773U,
    42467317U, 42532859U, 42598397U, 42663917U, 42729437U, 42795007U, 42860537U, 42926069U,
    42991609U, 43057151U, 43122683U, 43188203U, 43253759U, 43319293U, 43384813U, 43450357U,
    43515881U, 43581437U, 43646963U, 43712447U, 43778011U, 43843537U, 43909111U, 43974613U,
    44040187U, 44105689U, 44171261U, 44236799U, 44302303U, 44367833U, 44433391U, 44498941U,
    44564461U, 44630011U, 44695549U, 44761069U, 44826611U, 44892143U, 44957687U, 45023203U,
    45088739U, 45154283U, 45219827U, 45285371U, 45350869U, 45416411U, 45481973U, 45547517U,
    45613039U, 45678559U, 45744121U, 45809657U, 45875191U, 45940711U, 46006249U, 46071769U,
    46137319U, 46202867U, 46268381U, 46333943U, 46399471U, 46465019U, 46530557U, 46596089U,
    46661627U, 46727143U, 46792699U, 46858211U, 46923761U, 46989281U, 47054809U, 47120377U,
    47185907U, 47251447U, 47316991U, 47382527U, 47448061U, 47513563U, 47579131U, 47644669U,
    47710207U, 47775697U, 47841257U, 47906797U, 47972341U, 48037883U, 48103417U, 48168943U,
    48234451U, 48300029U, 48365563U, 48431063U, 48496639U, 48562127U, 48627697U, 48693223U,
    48758783U, 48824299U, 48889837U, 48955373U, 49020913U, 49086451U, 49151987U, 49217527U,
    49283063U, 49348601U, 49414111U, 49479653U, 49545193U, 49610741U, 49676267U, 49741807U,
    49807327U, 49872887U, 49938431U, 50003939U, 50069497U, 50135027U, 50200573U, 50266087U,
    50331599U, 50397167U, 50462683U, 50528227U, 50593783U, 50659321U, 50724859U, 50790391U,
    50855899U, 50921461U, 50987003U, 51052543U, 51118069U, 51183599U, 51249131U, 51314677U,
    51380179U, 51445747U, 51511277U, 51576829U, 51642341U, 51707893U, 51773431U, 51838949U,
    51904511U, 51970033U, 52035569U, 52101109U, 52166641U, 52232183U, 52297717U, 52363231U,
    52428767U, 52494329U, 52559867U, 52625407U, 52690919U, 52756453U, 52821983U, 52887533U,
    52953077U, 53018621U, 53084147U, 53149673U, 53215229U, 53280763U, 53346301U, 53411767U,
    53477357U, 53542901U, 53608441U, 53673979U, 53739493U, 53805049U, 53870573U, 53936111U,
    54001663U, 54067163U, 54132721U, 54198259U, 54263789U, 54329291U, 54394877U, 54460381U,
    54525917U, 54591479U, 54656983U, 54722557U, 54788089U, 54853583U, 54919159U, 54984679U,
    55050217U, 55115773U, 55181311U, 55246837U, 55312351U, 55377911U, 55443433U, 55508983U,
    55574507U, 55640063U, 55705589U, 55771091U, 55836659U, 55902181U, 55967701U, 56033203U,
    56098813U, 56164349U, 56229881U, 56295397U, 56360911U, 56426483U, 56491993U, 56557559U,
    56623093U, 56688637U, 56754167U, 56819699U, 56885219U, 56950741U, 57016319U, 57081811U,
    57147379U, 57212921U, 57278461U, 57343981U, 57409529U, 57475069U, 57540599U, 57606139U,
    57671671U, 57737209U, 57802739U, 57868273U, 57933817U, 57999341U, 58064861U, 58130393U,
    58195939U, 58261501U, 58327039U, 58392571U, 58458091U, 58523623U, 58589161U, 58654711U,
    58720253U, 58785781U, 58851307U, 58916863U, 58982389U, 59047927U, 59113469U, 59179007U,
    59244539U, 59310079U, 59375587U, 59441147U, 59506679U, 59572223U, 59637733U, 59703289U,
    59768831U, 59834329U, 59899901U, 59965429U, 60030953U, 60096481U, 60162029U, 60227581U,
    60293119U, 60358637U, 60424183U, 60489713U, 60555227U, 60620761U, 60686321U, 60751861U,
    60817397U, 60882929U, 60948479U, 61014001U, 61079531U, 61145087U, 61210603U, 61276151U,
    61341659U, 61407223U, 61472753U, 61538297U, 61603811U, 61669373U, 61734899U, 61800437U,
    61865971U, 61931491U, 61997053U, 62062573U, 62128127U, 62193641U, 62259193U, 62324729U,
    62390261U, 62455781U, 62521331U, 62586871U, 62652407U, 62717939U, 62783477U, 62849021U,
    62914549U, 62980069U, 63045613U, 63111151U, 63176693U, 63242239U, 63307763U, 63373309U,
    63438839U, 63504377U, 63569917U, 63635443U, 63700991U, 63766523U, 63832057U, 63897593U,
    63963131U, 64028669U, 64094207U, 64159723U, 64225267U, 64290799U, 64356349U, 64421881U,
    64487417U, 64552931U, 64618493U, 64684013U, 64749563U, 64815103U, 64880587U, 64946159U,
    65011703U, 65077231U, 65142769U, 65208307U, 65273851U, 65339387U, 65404909U, 65470453U,
    65535989U, 65601533U, 65667067U, 65732599U, 65798137U, 65863667U, 65929211U, 65994739U,
    66060277U, 66125819U, 66191351U, 66256891U, 66322427U, 66387967U, 66453479U, 66519023U,
    66584561U, 66650069U, 66715643U, 66781177U, 66846709U, 66912253U, 66977767U, 67043303U,
    67108859U, 67174397U, 67239883U, 67305463U, 67370999U, 67436539U, 67502063U, 67567609U,
    67633127U, 67698677U, 67764223U, 67829759U, 67895251U, 67960759U, 68026363U, 68091901U,
    68157433U, 68222969U, 68288503U, 68354029U, 68419567U, 68485073U, 68550631U, 68616187U,
    68681719U, 68747233U, 68812769U, 68878331U, 68943851U, 69009371U, 69074933U, 69140441U,
    69205987U, 69271541U, 69337087U, 69402601U, 69468151U, 69533647U, 69599221U, 69664753U,
    69730303U, 69795839U, 69861331U, 69926911U, 69992443U, 70057973U, 70123513U, 70189043U,
    70254563U, 70320127U, 70385641U, 70451191U, 70516729U, 70582271U, 70647793U, 70713343U,
    70778861U, 70844413U, 70909933U, 70975483U, 71041021U, 71106551U, 71172091U, 71237629U,
    71303153U, 71368657U, 71434229U, 71499763U, 71565283U, 71630837U, 71696363U, 71761919U,
    71827423U, 71892991U, 71958521U, 72024059U, 72089573U, 72155113U, 72220663U, 72286199U,
    72351733U, 72417277U, 72482807U, 72548351U, 72613861U, 72679421U, 72744937U, 72810469U,
    72876031U, 72941537U, 73007089U, 73072631U, 73138171U, 73203709U, 73269247U, 73334761U,
    73400311U, 73465853U, 73531379U, 73596917U, 73662461U, 73727981U, 73793521U, 73859069U,
    73924583U, 73990141U, 74055637U, 74121211U, 74186747U, 74252263U, 74317801U, 74383343U,
    74448877U, 74514403U, 74579951U, 74645479U, 74711027U, 74776553U, 74842099U, 74907643U,
    74973181U, 75038707U, 75104243U, 75169789U, 75235327U, 75300847U, 75366397U, 75431903U,
    75497467U, 75562979U, 75628513U, 75694063U, 75759613U, 75825137U, 75890653U, 75956203U,
    76021661U, 76087283U, 76152821U, 76218323U, 76283897U, 76349423U, 76414973U, 76480499U,
    76546039U, 76611583U, 76677113U, 76742629U, 76808119U, 76873703U, 76939253U, 77004793U,
    77070317U, 77135869U, 77201347U, 77266897U, 77332471U, 77397973U, 77463541U, 77529071U,
    77594599U, 77660129U, 77725691U, 77791223U, 77856767U, 77922253U, 77987821U, 78053363U,
    78118903U, 78184397U, 78249973U, 78315509U, 78381047U, 78446591U, 78512101U, 78577657U,
    78643199U, 78708733U, 78774259U, 78839807U, 78905303U, 78970877U, 79036411U, 79101947U,
    79167479U, 79233013U, 79298543U, 79364071U, 79429619U, 79495147U, 79560673U, 79626229U,
    79691761U, 79757309U, 79822829U, 79888381U, 79953901U, 80019449U, 80084969U, 80150507U,
    80216063U, 80281583U, 80347103U, 80412659U, 80478199U, 80543741U, 80609279U, 80674807U,
    80740339U, 80805887U, 80871419U, 80936959U, 81002489U, 81068027U, 81133567U, 81199099U,
    81264587U, 81330173U, 81395683U, 81461243U, 81526763U, 81592267U, 81657841U, 81723359U,
    81788923U, 81854449U, 81919993U, 81985529U, 82051043U, 82116589U, 82182137U, 82247677U,
    82313213U, 82378739U, 82444279U, 82509811U, 82575331U, 82640881U, 82706431U, 82771919U,
    82837501U, 82903039U, 82968563U, 83034073U, 83099641U, 83165183U, 83230717U, 83296253U,
    83361781U, 83427319U, 83492863U, 83558399U, 83623931U, 83689423U, 83754997U, 83820521U,
    83886053U, 83951611U, 84017117U, 84082667U, 84148213U, 84213721U, 84279277U, 84344809U,
    84410353U, 84475903U, 84541421U, 84606971U, 84672487U, 84738041U, 84803581U, 84869119U,
    84934621U, 85000183U, 85065719U, 85131247U, 85196789U, 85262297U, 85327849U, 85393381U,
    85458929U, 85524473U, 85589989U, 85655543U, 85721081U, 85786621U, 85852147U, 85917679U,
    85983217U, 86048759U, 86114279U, 86179823U, 86245343U, 86310883U, 86376443U, 86441959U,
    86507507U, 86572999U, 86638577U, 86704127U, 86769647U, 86835191U, 86900731U, 86966251U,
    87031759U, 87097331U, 87162857U, 87228413U, 87293939U, 87359483U, 87425021U, 87490553U,
    87556087U, 87621631U, 87687167U, 87752701U, 87818239U, 87883771U, 87949307U, 88014841U,
    88080359U, 88145903U, 88211449U, 88276973U, 88342519U, 88408057U, 88473569U, 88539133U,
    88604653U, 88670177U, 88735721U, 88801253U, 88866797U, 88932341U, 88997827U, 89063417U,
    89128939U, 89194481U, 89260027U, 89325559U, 89391103U, 89456599U, 89522171U, 89587709U,
    89653217U, 89718781U, 89784313U, 89849849U, 89915383U, 89980867U, 90046441U, 90111997U,
    90177533U, 90243061U, 90308599U, 90374143U, 90439667U, 90505211U, 90570751U, 90636277U,
    90701797U, 90767359U, 90832871U, 90898399U, 90963967U, 91029493U, 91095013U, 91160567U,
    91226101U, 91291643U, 91357177U, 91422707U, 91488251U, 91553771U, 91619321U, 91684847U,
    91750391U, 91815917U, 91881443U, 91947001U, 92012537U, 92078057U, 92143609U, 92209141U,
    92274671U, 92340223U, 92405723U, 92471287U, 92536823U, 92602351U, 92667863U, 92733437U,
    92798969U, 92864507U, 92930039U, 92995571U, 93061117U, 93126647U, 93192191U, 93257713U,
    93323249U, 93388759U, 93454307U, 93519871U, 93585379U, 93650941U, 93716471U, 93781993U,
    93847549U, 93913087U, 93978559U, 94044157U, 94109681U, 94175231U, 94240733U, 94306301U,
    94371833U, 94437373U, 94502899U, 94568437U, 94633963U, 94699489U, 94765039U, 94830583U,
    94896119U, 94961653U, 95027197U, 95092693U, 95158249U, 95223781U, 95289329U, 95354879U,
    95420401U, 95485933U, 95551487U, 95617013U, 95682541U, 95748089U, 95813621U, 95879153U,
    95944691U, 96010231U, 96075739U, 96141301U, 96206839U, 96272383U, 96337919U, 96403441U,
    96468979U, 96534523U, 96600041U, 96665599U, 96731101U, 96796633U, 96862169U, 96927739U,
    96993269U, 97058809U, 97124347U, 97189879U, 97255409U, 97320947U, 97386467U, 97452031U,
    97517543U, 97583099U, 97648637U, 97714153U, 97779701U, 97845239U, 97910759U, 97976299U,
    98041831U, 98107369U, 98172887U, 98238463U, 98303999U, 98369527U, 98435063U, 98500583U,
    98566121U, 98631607U, 98697187U, 98762747U, 98828281U, 98893807U, 98959337U, 99024869U,
    99090427U, 99155939U, 99221489U, 99287033U, 99352567U, 99418093U, 99483647U, 99549181U,
    99614689U, 99680249U, 99745787U, 99811319U, 99876851U, 99942397U, 100007927U, 100073471U,
    100138979U, 100204543U, 100270069U, 100335583U, 100401139U, 100466659U, 100532207U, 100597751U,
    100663291U, 100728821U, 100794319U, 100859903U, 100925431U, 100990957U, 101056507U, 101122037U,
    101187577U, 101253101U, 101318647U, 101384189U, 101449717U, 101515243U, 101580793U, 101646331U,
    101711839U, 101777393U, 101842931U, 101908379U, 101974009U, 102039551U, 102105049U, 102170617U,
    102236149U, 102301669U, 102367189U, 102432763U, 102498301U, 102563731U, 102629369U, 102694883U,
    102760387U, 102825971U, 102891499U, 102957053U, 103022537U, 103088123U, 103153649U, 103219199U,
    103284733U, 103350251U, 103415791U, 103481333U, 103546879U, 103612409U, 103677949U, 103743487U,
    103809011U, 103874557U, 103940093U, 104005621U, 104071157U, 104136677U, 104202233U, 104267773U,
    104333311U, 104398837U, 104464369U, 104529883U, 104595397U, 104660977U, 104726527U, 104792033U,
    104857589U, 104923123U, 104988641U, 105054197U, 105119741U, 105185221U, 105250811U, 105316349U,
    105381841U, 105447421U, 105512951U, 105578479U, 105644029U, 105709567U, 105775079U, 105840619U,
    105906167U, 105971711U, 106037237U, 106102769U, 106168319U, 106233851U, 106299379U, 106364927U,
    106430449U, 106495919U, 106561523U, 106627063U, 106692601U, 106758139U, 106823677U, 106889207U,
    106954747U, 107020279U, 107085799U, 107151353U, 107216891U, 107282423U, 107347943U, 107413489U,
    107479033U, 107544539U, 107610079U, 107675609U, 107741167U, 107806711U, 107872249U, 107937787U,
    108003323U, 108068861U, 108134393U, 108199933U, 108265459U, 108331007U, 108396521U, 108462073U,
    108527603U, 108593119U, 108658681U, 108724157U, 108789727U, 108855259U, 108920831U, 108986357U,
    109051903U, 109117439U, 109182947U, 109248497U, 109314043U, 109379549U, 109445107U, 109510649U,
    109576189U, 109641703U, 109707253U, 109772797U, 109838293U, 109903841U, 109969403U, 110034923U,
    110100409U, 110166013U, 110231531U, 110297069U, 110362559U, 110428159U, 110493661U, 110559203U,
    110624753U, 110690291U, 110755793U, 110821307U, 110886883U, 110952433U, 111017983U, 111083477U,
    111148963U, 111214589U, 111280121U, 111345649U, 111411173U, 111476731U, 111542261U, 111607807U,
    111673343U, 111738839U, 111804389U, 111869951U, 111935459U, 112001023U, 112066553U, 112132081U,
    112197629U, 112263167U, 112328683U, 112394239U, 112459751U, 112525241U, 112590839U, 112656359U,
    112721893U, 112787449U, 112852981U, 112918513U, 112984061U, 113049593U, 113115133U, 113180647U,
    113246183U, 113311733U, 113377279U, 113442793U, 113508319U, 113573881U, 113639419U, 113704957U,
    113770457U, 113836027U, 113901553U, 113967103U, 114032599U, 114098161U, 114163703U, 114229229U,
    114294721U, 114360319U, 114425807U, 114491387U, 114556913U, 114622463U, 114687977U, 114753497U,
    114819031U, 114884597U, 114950131U, 115015651U, 115081189U, 115146751U, 115212287U, 115277821U,
    115343341U, 115408879U, 115474417U, 115539923U, 115605467U, 115671037U, 115736539U, 115802111U,
    115867627U, 115933159U, 115998719U, 116064241U, 116129789U, 116195293U, 116260849U, 116326373U,
    116391917U, 116457449U, 116523007U, 116588513U, 116654077U, 116719607U, 116785133U, 116850683U,
    116916223U, 116981747U, 117047291U, 117112811U, 117178367U, 117243881U, 117309421U, 117374963U,
    117440509U, 117506003U, 117571523U, 117637103U, 117702649U, 117768191U, 117833711U, 117899251U,
    117964793U, 118030307U, 118095853U, 118161403U, 118226893U, 118292437U, 118358003U, 118423549U,
    118489081U, 118554619U, 118620143U, 118685629U, 118751207U, 118816739U, 118882279U, 118947839U,
    119013347U, 119078891U, 119144447U, 119209963U, 119275511U, 119341051U, 119406587U, 119472121U,
    119537653U, 119603189U, 119668723U, 119734267U, 119799803U, 119865341U, 119930873U, 119996411U,
    120061951U, 120127487U, 120193019U, 120258539U, 120324077U, 120389609U, 120455147U, 120520703U,
    120586231U, 120651763U, 120717307U, 120782839U, 120848353U, 120913901U, 120979447U, 121044967U,
    121110523U, 121176049U, 121241597U, 121307119U, 121372649U, 121438199U, 121503737U, 121569271U,
    121634801U, 121700333U, 121765871U, 121831399U, 121896949U, 121962493U, 122028019U, 122093557U,
    122159101U, 122224639U, 122290171U, 122355697U, 122421241U, 122486759U, 122552317U, 122617837U,
    122683391U, 122748919U, 122814463U, 122879971U, 122945527U, 123011071U, 123076601U, 123142141U,
    123207677U, 123273211U, 123338737U, 123404279U, 123469783U, 123535339U, 123600857U, 123666407U,
    123731963U, 123797437U, 123863023U, 123928561U, 123994099U, 124059647U, 124125161U, 124190701U,
    124256243U, 124321759U, 124387321U, 124452863U, 124518397U, 124583933U, 124649449U, 124714999U,
    124780531U, 124846063U, 124911601U, 124977133U, 125042663U, 125108213U, 125173759U, 125239291U,
    125304787U, 125370367U, 125435897U, 125501417U, 125566963U, 125632483U, 125698021U, 125763577U,
    125829103U, 125894647U, 125960189U, 126025723U, 126091241U, 126156773U, 126222293U, 126287809U,
    126353407U, 126418933U, 126484469U, 126550009U, 126615551U, 126681073U, 126746623U, 126812159U,
    126877693U, 126943211U, 127008733U, 127074253U, 127139833U, 127205327U, 127270849U, 127336439U,
    127401947U, 127467517U, 127533047U, 127598543U, 127664113U, 127729643U, 127795181U, 127860721U,
    127926263U, 127991807U, 128057327U, 128122861U, 128188409U, 128253949U, 128319469U, 128385017U,
    128450533U, 128516083U, 128581631U, 128647163U, 128712691U, 128778227U, 128843761U, 128909311U,
    128974841U, 129040367U, 129105901U, 129171439U, 129236959U, 129302497U, 129368051U, 129433571U,
    129499129U, 129564647U, 129630199U, 129695711U, 129761273U, 129826813U, 129892333U, 129957871U,
    130023407U, 130088951U, 130154483U, 130220029U, 130285567U, 130351079U, 130416631U, 130482173U,
    130547621U, 130613221U, 130678781U, 130744319U, 130809853U, 130875389U, 130940911U, 131006461U,
    131071987U, 131137493U, 131203069U, 131268607U, 131334131U, 131399623U, 131465177U, 131530741U,
    131596279U, 131661809U, 131727359U, 131792887U, 131858413U, 131923951U, 131989477U, 132055037U,
    132120557U, 132186107U, 132251621U, 132317131U, 132382717U, 132448247U, 132513781U, 132579319U,
    132644851U, 132710387U, 132775931U, 132841441U, 132907007U, 132972509U, 133038053U, 133103611U,
    133169137U, 133234687U, 133300207U, 133365737U, 133431293U, 133496813U, 133562329U, 133627883U,
    133693433U, 133758967U, 133824503U, 133890047U, 133955581U, 134021101U, 134086639U, 134152189U,
    134217689U, 134283257U, 134348789U, 134414327U, 134479871U, 134545399U, 134610929U, 134676469U,
    134742007U, 134807503U, 134873083U, 134938619U, 135004141U, 135069679U, 135135229U, 135200753U,
    135266293U, 135331837U, 135397373U, 135462907U, 135528439U, 135593957U, 135659483U, 135724973U,
    135790547U, 135856121U, 135921661U, 135987193U, 136052723U, 136118249U, 136183807U, 136249343U,
    136314869U, 136380403U, 136445951U, 136511471U, 136577011U, 136642559U, 136708093U, 136773613U,
    136839133U, 136904701U, 136970233U, 137035763U, 137101297U, 137166823U, 137232353U, 137297903U,
    137363431U, 137428961U, 137494501U, 137560061U, 137625599U, 137691061U, 137756659U, 137822177U,
    137887741U, 137953273U, 138018781U, 138084329U, 138149831U, 138215417U, 138280957U, 138346489U,
    138412031U, 138477551U, 138543103U, 138608579U, 138674171U, 138739633U, 138805237U, 138870769U,
    138936319U, 139001851U, 139067387U, 139132909U, 139198459U, 139263953U, 139329523U, 139395031U,
    139460591U, 139526129U, 139591663U, 139657183U, 139722749U, 139788283U, 139853807U, 139919357U,
    139984829U, 140050399U, 140115967U, 140181491U, 140246947U, 140312567U, 140378107U, 140443627U,
    140509183U, 140574719U, 140640251U, 140705779U, 140771317U, 140836849U, 140902369U, 140967923U,
    141033463U, 141099001U, 141164503U, 141230077U, 141295603U, 141361139U, 141426667U, 141492223U,
    141557723U, 141623291U, 141688831U, 141754367U, 141819901U, 141885439U, 141950971U, 142016473U,
    142082023U, 142147553U, 142213091U, 142278641U, 142344131U, 142409723U, 142475261U, 142540787U
};


namespace riner {

    uint32_t DagFile::getEpoch() const {
        return epoch;
    }

    uint32_t DagFile::getSize(uint32_t epoch) {
        return epoch < 2048 ? dagSize[epoch] : std::numeric_limits<uint32_t>::max();
    }

    uint64_t DagFile::getByteSize(uint32_t epoch) {
        return epoch < 2048 ? uint64_t(128) * dagSize[epoch] : std::numeric_limits<uint64_t>::max();
    }

    DagFile::operator bool() const {
        return valid;
    }

    bool DagFile::generate(const DagCacheContainer &cache,
                  const cl::Context &context, const cl::Device &device, cl::Program &generateDagProgram) {
        cl_int err;
        auto epoch = cache.getEpoch();

        //allocate dag memory on the gpu
        const auto futureEpochs = 6; //preallocate this far ahead

        if (allocationMaxEpoch < epoch) {
            allocationMaxEpoch = epoch + futureEpochs;

            auto allocSize = getByteSize(allocationMaxEpoch);
            auto clDeviceMaxMemAllocSize = device.getInfo<CL_DEVICE_MAX_MEM_ALLOC_SIZE>();
            if (allocSize > clDeviceMaxMemAllocSize) {
                LOG(WARNING) << "trying to allocate clDagBuffer of size " << allocSize << " which exceeds " << clDeviceMaxMemAllocSize << " = CL_DEVICE_MAX_MEM_ALLOC_SIZE";
            }

            LOG(INFO) << "allocating clDagBuffer size = " << allocSize;
            clDagBuffer = cl::Buffer{context,
                                     CL_MEM_READ_WRITE |
                                     CL_MEM_HOST_NO_ACCESS,
                                     allocSize, nullptr, &err
            };

            if (err) {
                LOG(ERROR) << "#" << err << " allocation of clDagBuffer failed (size = " << allocSize << " on device " << device() << ")";
#ifdef RNR_PLATFORM_LINUX
                if (auto id = obtainDeviceIdFromOpenCLDevice(device)) {
                    if (id->getVendor() == kAMD) {
                        LOG(ERROR) << "allocation failed maybe because the following env variables are not defined like so: \n"
                                     "export GPU_MAX_HEAP_SIZE=100\n"
                                     "export GPU_USE_SYNC_OBJECTS=1\n"
                                     "export GPU_MAX_ALLOC_PERCENT=100\n"
                                     "export GPU_SINGLE_ALLOC_PERCENT=100\n";
                    }
                }
#endif
                return false;
            }
            else {
                LOG(INFO) << "allocating clDagBuffer successful";
            }

        }

        if (!clDagBuffer()) {
            LOG(ERROR) << "clDagBuffer not valid";
            return false;
        }

        //upload dag cache which is required for dag creation
        auto cacheSpan = cache.getByteCache();

        VLOG(2) << "creating clDagCache";
        cl::Buffer clDagCache{context,
            CL_MEM_READ_ONLY |
            CL_MEM_COPY_HOST_PTR |
            CL_MEM_HOST_WRITE_ONLY,
            (size_t)cacheSpan.size(),
            (void *)cacheSpan.data(), &err
        };

        if (err) {
            LOG(ERROR) << "#" << err << " creation of clDagCache failed (size = " << cacheSpan.size() << " on device " << device() << ")";
            return false;
        }
        else VLOG(2) << "creating clDagCache successful";

        auto genDagKernel = cl::Kernel(generateDagProgram, "GenerateDAG", &err);
        if (err) {
            LOG(ERROR) << "#" << err << " could not get dag generation kernel from cl program";
            return false;
        }

        cl::CommandQueue cmdQueue(context, device, 0, &err);
        if (err) {
            LOG(ERROR) << "#" << err << " failed creating a command queue for dag generation";
            return false;
        }

        //run the kernel
        cl_uint arg = 0;

        cl_uint zero = 0;
        cl_uint cacheSize = cache.getSize();
        cl_uint isolate = UINT32_MAX;

        genDagKernel.setArg(arg++, zero);
        genDagKernel.setArg(arg++, clDagCache);
        genDagKernel.setArg(arg++, clDagBuffer);
        genDagKernel.setArg(arg++, cacheSize);
        genDagKernel.setArg(arg++, isolate);

        this->size = getSize(epoch);
        cl::NDRange offset{0};
        cl::NDRange dagItems{2 * this->size};

        err = cmdQueue.enqueueNDRangeKernel(genDagKernel, offset, dagItems);
        if (err) {
            LOG(ERROR) << "#" << err << " error when enqueueing GenerateDAG kernel on " << std::this_thread::get_id();
        }
        cmdQueue.finish(); //clFinish();

        clDagCache = {}; //clReleaseMemObject

        this->epoch = epoch;
        RNR_EXPECTS(this->size != 0);
        valid = true;
        return true;
    }

    cl::Buffer &DagFile::getCLBuffer() {
        RNR_EXPECTS(valid);
        return clDagBuffer;
    }

    uint32_t DagFile::getSize() const {
        return valid ? size : std::numeric_limits<uint32_t>::max();
    }

    uint64_t DagFile::getByteSize() const {
        return valid ? uint64_t(128) * size : std::numeric_limits<uint64_t>::max();
    }

}
