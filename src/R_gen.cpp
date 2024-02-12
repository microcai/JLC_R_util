
#include <stdexcept>
#include "R_gen.hpp"

static const int E24_R[] = {
    10,      11,      12,      13,      15,      16,      18,      20,
    22,      24,      27,      30,      33,      36,      39,      43,
    47,      51,      56,      62,      68,      75,      82,      91,

    100,     110,     120,     130,     150,     160,     180,     200,
    220,     240,     270,     300,     330,     360,     390,     430,
    470,     510,     560,     620,     680,     750,     820,     910,

    1000,    1100,    1200,    1300,    1500,    1600,    1800,    2000,
    2200,    2400,    2700,    3000,    3300,    3600,    3900,    4300,
    4700,    5100,    5600,    6200,    6800,    7500,    8200,    9100,

    10000,   11000,   12000,   13000,   15000,   16000,   18000,   20000,
    22000,   24000,   27000,   30000,   33000,   36000,   39000,   43000,
    47000,   51000,   56000,   62000,   68000,   75000,   82000,   91000,

    100000,  110000,  120000,  130000,  150000,  160000,  180000,  200000,
    220000,  240000,  270000,  300000,  330000,  360000,  390000,  430000,
    470000,  510000,  560000,  620000,  680000,  750000,  820000,  910000,

    1000000, 1100000, 1200000, 1300000, 1500000, 1600000, 1800000, 2000000,
    2200000, 2400000, 2700000, 3000000, 3300000, 3600000, 3900000, 4300000,
    4700000, 5100000, 5600000, 6200000, 6800000, 7500000, 8200000, 9100000,
};

static const int E96_R[] = {
    100,    102,    105,    107,    110,    113,    115,    118,    121,
    124,    127,    130,    133,    137,    140,    143,    147,    150,
    154,    158,    162,    165,    169,    174,    178,    182,    187,
    191,    196,    200,    205,    210,    215,    221,    226,    232,
    237,    243,    249,    255,    261,    267,    274,    280,    287,
    294,    301,    309,    316,    324,    332,    340,    348,    357,
    365,    374,    383,    392,    402,    412,    422,    432,    442,
    453,    464,    475,    487,    499,    511,    523,    536,    549,
    562,    576,    590,    604,    619,    634,    649,    665,    681,
    698,    715,    732,    750,    768,    787,    806,    825,    845,
    866,    887,    908,    931,    953,    976,

    1000,   1020,   1050,   1070,   1100,   1130,   1150,   1180,   1210,
    1240,   1270,   1300,   1330,   1370,   1400,   1430,   1470,   1500,
    1540,   1580,   1620,   1650,   1690,   1740,   1780,   1820,   1870,
    1910,   1960,   2000,   2050,   2100,   2150,   2210,   2260,   2320,
    2370,   2430,   2490,   2550,   2610,   2670,   2740,   2800,   2870,
    2940,   3010,   3090,   3160,   3240,   3320,   3400,   3480,   3570,
    3650,   3740,   3830,   3920,   4020,   4120,   4220,   4320,   4420,
    4530,   4640,   4750,   4870,   4990,   5110,   5230,   5360,   5490,
    5620,   5760,   5900,   6040,   6190,   6340,   6490,   6650,   6810,
    6980,   7150,   7320,   7500,   7680,   7870,   8060,   8250,   8450,
    8660,   8870,   9090,   9310,   9530,   9760,

    10000,  10200,  10500,  10700,  11000,  11300,  11500,  11800,  12100,
    12400,  12700,  13000,  13300,  13700,  14000,  14300,  14700,  15000,
    15400,  15800,  16200,  16500,  16900,  17400,  17800,  18200,  18700,
    19100,  19600,  20000,  20500,  21000,  21500,  22100,  22600,  23200,
    23700,  24300,  24900,  25500,  26100,  26700,  27400,  28000,  28700,
    29400,  30100,  30900,  31600,  32400,  33200,  34000,  34800,  35700,
    36500,  37400,  38300,  39200,  40200,  41200,  42200,  43200,  44200,
    45300,  46400,  47500,  48700,  49900,  51100,  52300,  53600,  54900,
    56200,  57600,  59000,  60400,  61900,  63400,  64900,  66500,  68100,
    69800,  71500,  73200,  75000,  76800,  78700,  80600,  82500,  84500,
    86600,  88700,  90900,  93100,  95300,  97600,

    100000, 102000, 105000, 107000, 110000, 113000, 115000, 118000, 121000,
    124000, 127000, 130000, 133000, 137000, 140000, 143000, 147000, 150000,
    154000, 158000, 162000, 165000, 169000, 174000, 178000, 182000, 187000,
    191000, 196000, 200000, 205000, 210000, 215000, 221000, 226000, 232000,
    237000, 243000, 249000, 255000, 261000, 267000, 274000, 280000, 287000,
    294000, 301000, 309000, 316000, 324000, 332000, 340000, 348000, 357000,
    365000, 374000, 383000, 392000, 402000, 412000, 422000, 432000, 442000,
    453000, 464000, 475000, 487000, 499000, 511000, 523000, 536000, 549000,
    562000, 576000, 590000, 604000, 619000, 634000, 649000, 665000, 681000,
    698000, 715000, 732000, 750000, 768000, 787000, 806000, 825000, 845000,
    866000, 887000, 909000, 931000, 953000, 976000,
};

static const struct Resistance JLC_free[] = {
{120,	"120Ω ±1% 125mW 厚膜电阻",	"C17437"},
{120,	"120Ω ±1% 250mW 厚膜电阻",	"C17909"},
{120,	"120Ω ±1% 62.5mW 厚膜电阻",	"C25079"},
{150,	"150Ω ±1% 100mW 厚膜电阻",	"C22808"},
{150,	"150Ω ±1% 125mW 厚膜电阻",	"C17471"},
{150,	"150Ω ±1% 62.5mW 厚膜电阻",	"C25082"},
{180,	"180Ω ±1% 100mW 厚膜电阻",	"C22828"},
{180,	"180Ω ±1% 125mW 厚膜电阻",	"C25270"},
{180,	"180Ω ±1% 250mW 厚膜电阻",	"C17924"},
{200,	"200Ω ±1% 100mW 厚膜电阻",	"C8218"},
{200,	"200Ω ±1% 125mW 厚膜电阻",	"C17540"},
{200,	"200Ω ±1% 62.5mW 厚膜电阻",	"C25087"},
{220,	"220Ω ±1% 100mW 厚膜电阻",	"C22962"},
{220,	"220Ω ±1% 125mW 厚膜电阻",	"C17557"},
{220,	"220Ω ±1% 62.5mW 厚膜电阻",	"C25091"},
{240,	"240Ω ±1% 100mW 厚膜电阻",	"C23350"},
{240,	"240Ω ±1% 125mW 厚膜电阻",	"C17572"},
{270,	"270Ω ±1% 100mW 厚膜电阻",	"C22966"},
{270,	"270Ω ±1% 125mW 厚膜电阻",	"C17590"},
{300,	"300Ω ±1% 100mW 厚膜电阻",	"C23025"},
{300,	"300Ω ±1% 125mW 厚膜电阻",	"C17617"},
{300,	"300Ω ±1% 250mW 厚膜电阻",	"C17887"},
{300,	"300Ω ±1% 62.5mW 厚膜电阻",	"C25102"},
{330,	"330Ω ±1% 100mW 厚膜电阻",	"C23138"},
{330,	"330Ω ±1% 125mW 厚膜电阻",	"C17630"},
{330,	"330Ω ±1% 62.5mW 厚膜电阻",	"C25104"},
{360,	"360Ω ±1% 100mW 厚膜电阻",	"C25194"},
{390,	"390Ω ±1% 100mW 厚膜电阻",	"C23151"},
{390,	"390Ω ±1% 125mW 厚膜电阻",	"C17655"},
{430,	"430Ω ±1% 100mW 厚膜电阻",	"C23170"},
{470,	"470Ω ±1% 100mW 厚膜电阻",	"C23179"},
{470,	"470Ω ±1% 125mW 厚膜电阻",	"C17710"},
{470,	"470Ω ±1% 62.5mW 厚膜电阻",	"C25117"},
{510,	"510Ω ±1% 100mW 厚膜电阻",	"C23193"},
{510,	"510Ω ±1% 125mW 厚膜电阻",	"C17734"},
{510,	"510Ω ±1% 62.5mW 厚膜电阻",	"C25123"},
{560,	"560Ω ±1% 100mW 厚膜电阻",	"C23204"},
{560,	"560Ω ±1% 125mW 厚膜电阻",	"C28636"},
{620,	"620Ω ±1% 100mW 厚膜电阻",	"C23220"},
{680,	"680Ω ±1% 100mW 厚膜电阻",	"C23228"},
{680,	"680Ω ±1% 125mW 厚膜电阻",	"C17798"},
{680,	"680Ω ±1% 62.5mW 厚膜电阻",	"C25130"},
{750,	"750Ω ±1% 100mW 厚膜电阻",	"C23241"},
{750,	"750Ω ±1% 125mW 厚膜电阻",	"C17818"},
{750,	"750Ω ±1% 250mW 厚膜电阻",	"C17985"},
{820,	"820Ω ±1% 100mW 厚膜电阻",	"C23253"},
{820,	"820Ω ±1% 125mW 厚膜电阻",	"C17837"},
{1000,	"1kΩ ±1% 100mW 厚膜电阻",	"C21190"},
{1000,	"1kΩ ±1% 125mW 厚膜电阻",	"C17513"},
{1000,	"1kΩ ±1% 250mW 厚膜电阻",	"C4410"},
{1000,	"1kΩ ±1% 62.5mW 厚膜电阻",	"C11702"},
{1100,	"1.1kΩ ±1% 100mW 厚膜电阻",	"C22764"},
{1200,	"1.2kΩ ±1% 100mW 厚膜电阻",	"C22765"},
{1200,	"1.2kΩ ±1% 125mW 厚膜电阻",	"C17379"},
{1200,	"1.2kΩ ±1% 62.5mW 厚膜电阻",	"C25862"},
{1500,	"1.5kΩ ±1% 100mW 厚膜电阻",	"C22843"},
{1500,	"1.5kΩ ±1% 125mW 厚膜电阻",	"C4310"},
{1500,	"1.5kΩ ±1% 62.5mW 厚膜电阻",	"C25867"},
{1800,	"1.8kΩ ±1% 100mW 厚膜电阻",	"C4177"},
{1800,	"1.8kΩ ±1% 125mW 厚膜电阻",	"C17398"},
{2000,	"2kΩ ±1% 100mW 厚膜电阻",	"C22975"},
{2000,	"2kΩ ±1% 125mW 厚膜电阻",	"C17604"},
{2000,	"2kΩ ±1% 250mW 厚膜电阻",	"C17944"},
{2000,	"2kΩ ±1% 62.5mW 厚膜电阻",	"C4109"},
{2200,	"2.2kΩ ±1% 100mW 厚膜电阻",	"C4190"},
{2200,	"2.2kΩ ±1% 125mW 厚膜电阻",	"C17520"},
{2200,	"2.2kΩ ±1% 62.5mW 厚膜电阻",	"C25879"},
{2400,	"2.4kΩ ±1% 100mW 厚膜电阻",	"C22940"},
{2400,	"2.4kΩ ±1% 125mW 厚膜电阻",	"C17526"},
{2400,	"2.4kΩ ±1% 62.5mW 厚膜电阻",	"C25882"},
{2700,	"2.7kΩ ±1% 100mW 厚膜电阻",	"C13167"},
{2700,	"2.7kΩ ±1% 125mW 厚膜电阻",	"C17530"},
{3000,	"3kΩ ±1% 100mW 厚膜电阻",	"C4211"},
{3000,	"3kΩ ±1% 125mW 厚膜电阻",	"C17661"},
{3300,	"3.3kΩ ±1% 100mW 厚膜电阻",	"C22978"},
{3300,	"3.3kΩ ±1% 125mW 厚膜电阻",	"C26010"},
{3300,	"3.3kΩ ±1% 62.5mW 厚膜电阻",	"C25890"},
{3600,	"3.6kΩ ±1% 100mW 厚膜电阻",	"C22980"},
{3600,	"3.6kΩ ±1% 125mW 厚膜电阻",	"C18359"},
{3900,	"3.9kΩ ±1% 100mW 厚膜电阻",	"C23018"},
{3900,	"3.9kΩ ±1% 125mW 厚膜电阻",	"C17614"},
{3900,	"3.9kΩ ±1% 62.5mW 厚膜电阻",	"C51721"},
{4300,	"4.3kΩ ±1% 100mW 厚膜电阻",	"C23159"},
{4300,	"4.3kΩ ±1% 125mW 厚膜电阻",	"C17667"},
{4300,	"43kΩ ±1% 100mW 厚膜电阻",	"C23172"},
{4300,	"43kΩ ±1% 125mW 厚膜电阻",	"C17695"},
{4700,	"4.7kΩ ±1% 100mW 厚膜电阻",	"C23162"},
{4700,	"4.7kΩ ±1% 125mW 厚膜电阻",	"C17673"},
{4700,	"4.7kΩ ±1% 250mW 厚膜电阻",	"C17936"},
{4700,	"4.7kΩ ±1% 62.5mW 厚膜电阻",	"C25900"},
{4990,	"4.99kΩ ±1% 100mW 厚膜电阻",	"C23046"},
{4990,	"4.99kΩ ±1% 125mW 厚膜电阻",	"C17677"},
{5100,	"5.1kΩ ±1% 100mW 厚膜电阻",	"C23186"},
{5100,	"5.1kΩ ±1% 125mW 厚膜电阻",	"C27834"},
{5100,	"5.1kΩ ±1% 62.5mW 厚膜电阻",	"C25905"},
{5600,	"5.6kΩ ±1% 100mW 厚膜电阻",	"C23189"},
{5600,	"5.6kΩ ±1% 125mW 厚膜电阻",	"C4382"},
{5600,	"5.6kΩ ±1% 62.5mW 厚膜电阻",	"C25908"},
{6200,	"6.2kΩ ±1% 100mW 厚膜电阻",	"C4260"},
{6200,	"6.2kΩ ±1% 125mW 厚膜电阻",	"C17767"},
{6800,	"6.8kΩ ±1% 100mW 厚膜电阻",	"C23212"},
{6800,	"6.8kΩ ±1% 125mW 厚膜电阻",	"C17772"},
{6800,	"6.8kΩ ±1% 62.5mW 厚膜电阻",	"C25917"},
{7500,	"7.5kΩ ±1% 100mW 厚膜电阻",	"C23234"},
{7500,	"7.5kΩ ±1% 125mW 厚膜电阻",	"C17807"},
{7500,	"7.5kΩ ±1% 62.5mW 厚膜电阻",	"C25918"},
{8200,	"8.2kΩ ±1% 100mW 厚膜电阻",	"C25981"},
{8200,	"8.2kΩ ±1% 125mW 厚膜电阻",	"C17828"},
{8200,	"8.2kΩ ±1% 62.5mW 厚膜电阻",	"C25924"},
{9100,	"9.1kΩ ±1% 100mW 厚膜电阻",	"C23260"},
{9100,	"9.1kΩ ±1% 125mW 厚膜电阻",	"C17855"},
{10000,	"10kΩ ±1% 125mW 厚膜电阻",	"C17414"},
{10000,	"10kΩ ±1% 250mW 厚膜电阻",	"C17902"},
{10000,	"10kΩ ±1% 62.5mW 厚膜电阻",	"C25744"},
{10000,	"厚膜电阻 10kΩ ±1% 100mW",	"C25804"},
{11000,	"11kΩ ±1% 100mW 厚膜电阻",	"C25950"},
{11000,	"11kΩ ±1% 125mW 厚膜电阻",	"C17429"},
{12000,	"12kΩ ±1% 100mW 厚膜电阻",	"C22790"},
{12000,	"12kΩ ±1% 125mW 厚膜电阻",	"C17444"},
{12000,	"12kΩ ±1% 62.5mW 厚膜电阻",	"C25752"},
{13000,	"13kΩ ±1% 100mW 厚膜电阻",	"C22797"},
{13000,	"13kΩ ±1% 125mW 厚膜电阻",	"C17455"},
{15000,	"15kΩ ±1% 100mW 厚膜电阻",	"C22809"},
{15000,	"15kΩ ±1% 125mW 厚膜电阻",	"C17475"},
{15000,	"15kΩ ±1% 62.5mW 厚膜电阻",	"C25756"},
{16000,	"16kΩ ±1% 125mW 厚膜电阻",	"C17490"},
{18000,	"18kΩ ±1% 100mW 厚膜电阻",	"C25810"},
{18000,	"18kΩ ±1% 125mW 厚膜电阻",	"C17506"},
{18000,	"18kΩ ±1% 62.5mW 厚膜电阻",	"C25762"},
{20000,	"20kΩ ±1% 100mW 厚膜电阻",	"C4184"},
{20000,	"20kΩ ±1% 125mW 厚膜电阻",	"C4328"},
{20000,	"20kΩ ±1% 62.5mW 厚膜电阻",	"C25765"},
{22000,	"22kΩ ±1% 100mW 厚膜电阻",	"C31850"},
{22000,	"22kΩ ±1% 125mW 厚膜电阻",	"C17560"},
{22000,	"22kΩ ±1% 62.5mW 厚膜电阻",	"C25768"},
{24000,	"24kΩ ±1% 100mW 厚膜电阻",	"C23352"},
{24000,	"24kΩ ±1% 125mW 厚膜电阻",	"C17575"},
{24000,	"24kΩ ±1% 62.5mW 厚膜电阻",	"C25769"},
{27000,	"27kΩ ±1% 100mW 厚膜电阻",	"C22967"},
{27000,	"27kΩ ±1% 125mW 厚膜电阻",	"C17593"},
{27000,	"27kΩ ±1% 62.5mW 厚膜电阻",	"C25771"},
{30000,	"30kΩ ±1% 100mW 厚膜电阻",	"C22984"},
{30000,	"30kΩ ±1% 125mW 厚膜电阻",	"C17621"},
{33000,	"33kΩ ±1% 100mW 厚膜电阻",	"C4216"},
{33000,	"33kΩ ±1% 125mW 厚膜电阻",	"C17633"},
{33000,	"33kΩ ±1% 62.5mW 厚膜电阻", "C25779"},
{36000,	"36kΩ ±1% 100mW 厚膜电阻",  "C23147"},
{36000,	"36kΩ ±1% 125mW 厚膜电阻",  "C4360"},
{39000,	"39kΩ ±1% 100mW 厚膜电阻",  "C23153"},
{39000,	"39kΩ ±1% 125mW 厚膜电阻",  "C25826"},
{39000,	"39kΩ ±1% 62.5mW 厚膜电阻", "C25783"},
{40200,	"40.2kΩ ±1% 100mW 厚膜电阻",    "C12447"},
{47000,	"47kΩ ±1% 100mW 厚膜电阻",  "C25819"},
{47000,	"47kΩ ±1% 125mW 厚膜电阻",  "C17713"},
{47000,	"47kΩ ±1% 62.5mW 厚膜电阻", "C25792"},
{49900,	"49.9kΩ ±1% 100mW 厚膜电阻",    "C23184"},
{49900,	"49.9kΩ ±1% 125mW 厚膜电阻",    "C17719"},
{49900,	"49.9kΩ ±1% 62.5mW 厚膜电阻",   "C25897"},
{51000,	"51kΩ ±1% 100mW 厚膜电阻",  "C23196"},
{51000,	"51kΩ ±1% 125mW 厚膜电阻",  "C17737"},
{51000,	"51kΩ ±1% 62.5mW 厚膜电阻", "C25794"},
{56000,	"56kΩ ±1% 100mW 厚膜电阻",  "C23206"},
{56000,	"56kΩ ±1% 125mW 厚膜电阻",  "C17756"},
{56000,	"56kΩ ±1% 62.5mW 厚膜电阻", "C25796"},
{62000,	"62kΩ ±1% 100mW 厚膜电阻", "C23221"},
{62000,	"62kΩ ±1% 125mW 厚膜电阻", "C17783"},
{68000,	"68kΩ ±1% 100mW 厚膜电阻", "C23231"},
{68000,	"68kΩ ±1% 125mW 厚膜电阻", "C17801"},
{68000,	"68kΩ ±1% 62.5mW 厚膜电阻"	,"C36871"},
{75000,	"75kΩ ±1% 100mW 厚膜电阻"	,"C23242"},
{75000,	"75kΩ ±1% 125mW 厚膜电阻"	,"C17819"},
{75000,	"75kΩ ±1% 62.5mW 厚膜电阻"	,"C25798"},
{82000,	"82kΩ ±1% 100mW 厚膜电阻"	,"C23254"},
{82000,	"82kΩ ±1% 125mW 厚膜电阻"	,"C17840"},
{91000,	"91kΩ ±1% 100mW 厚膜电阻"	,"C23265"},
{100000,	"100kΩ ±1% 100mW 厚膜电阻"	,"C25803"},
{100000,	"100kΩ ±1% 125mW 厚膜电阻"	,"C14950"},
{100000,	"100kΩ ±1% 250mW 厚膜电阻"	,"C17900"},
{100000,	"100kΩ ±1% 62.5mW 厚膜电阻"	,"C25741"},
{110000,	"110kΩ ±1% 100mW 厚膜电阻"	,"C25805"},
{120000,	"120kΩ ±1% 100mW 厚膜电阻"	,"C25808"},
{120000,	"120kΩ ±1% 125mW 厚膜电阻"	,"C17436"},
{120000,	"120kΩ ±1% 62.5mW 厚膜电阻"	,"C25750"},
{130000,	"130kΩ ±1% 100mW 厚膜电阻"	,"C22795"},
{150000,	"150kΩ ±1% 100mW 厚膜电阻"	,"C22807"},
{150000,	"150kΩ ±1% 125mW 厚膜电阻"	,"C17470"},
{150000,	"150kΩ ±1% 62.5mW 厚膜电阻"	,"C25755"},
{160000,	"160kΩ ±1% 100mW 厚膜电阻"	,"C22813"},
{180000,	"180kΩ ±1% 100mW 厚膜电阻"	,"C22827"},
{180000,	"180kΩ ±1% 125mW 厚膜电阻"	,"C17501"},
{200000,	"200kΩ ±1% 100mW 厚膜电阻"	,"C25811"},
{200000,	"200kΩ ±1% 125mW 厚膜电阻"	,"C17539"},
{200000,	"200kΩ ±1% 62.5mW 厚膜电阻"	,"C25764"},
{220000,	"220kΩ ±1% 100mW 厚膜电阻"	,"C22961"},
{220000,	"220kΩ ±1% 125mW 厚膜电阻"	,"C17556"},
{220000,	"220kΩ ±1% 62.5mW 厚膜电阻"	,"C25767"},
{240000,	"240kΩ ±1% 100mW 厚膜电阻"	,"C4197"},
{270000,	"270kΩ ±1% 100mW 厚膜电阻"	,"C22965"},
{270000,	"270kΩ ±1% 125mW 厚膜电阻"	,"C17589"},
{300000,	"300kΩ ±1% 100mW 厚膜电阻"	,"C23024"},
{300000,	"300kΩ ±1% 125mW 厚膜电阻"	,"C17616"},
{300000,	"300kΩ ±1% 62.5mW 厚膜电阻"	,"C25774"},
{330000,	"330kΩ ±1% 100mW 厚膜电阻"	,"C23137"},
{330000,	"330kΩ ±1% 125mW 厚膜电阻"	,"C17629"},
{330000,	"330kΩ ±1% 62.5mW 厚膜电阻"	,"C25778"},
{360000,	"360kΩ ±1% 100mW 厚膜电阻"	,"C23146"},
{390000,	"390kΩ ±1% 100mW 厚膜电阻"	,"C23150"},
{390000,	"390kΩ ±1% 125mW 厚膜电阻"	,"C17656"},
{430000,	"430kΩ ±1% 100mW 厚膜电阻"	,"C25969"},
{470000,	"470kΩ ±1% 100mW 厚膜电阻"	,"C23178"},
{470000,	"470kΩ ±1% 125mW 厚膜电阻"	,"C17709"},
{470000,	"470kΩ ±1% 62.5mW 厚膜电阻"	,"C25790"},
{510000,	"510kΩ ±1% 100mW 厚膜电阻"	,"C23192"},
{510000,	"510kΩ ±1% 125mW 厚膜电阻"	,"C17733"},
{510000,	"510kΩ ±1% 62.5mW 厚膜电阻"	,"C11616"},
{560000,	"560kΩ ±1% 100mW 厚膜电阻"	,"C23203"},
{680000,	"680kΩ ±1% 100mW 厚膜电阻"	,"C25822"},
{680000,	"680kΩ ±1% 125mW 厚膜电阻"	,"C17797"},
{750000,	"750kΩ ±1% 100mW 厚膜电阻"	,"C23240"},
{1000000, "1MΩ ±1% 100mW 厚膜电阻",	"C22935"},
{1000000, "1MΩ ±1% 125mW 厚膜电阻",	"C17514"},
{1000000, "1MΩ ±1% 250mW 厚膜电阻",	"C17927"},
{1000000, "1MΩ ±1% 62.5mW 厚膜电阻",	"C26083"},
{1200000, "1.2MΩ ±1% 250mW 厚膜电阻",	"C22107"},
{1500000, "1.5MΩ ±1% 100mW 厚膜电阻",	"C4172"},
{2000000, "2MΩ ±1% 100mW 厚膜电阻",	"C22976"},
{2000000, "2MΩ ±1% 125mW 厚膜电阻",	"C26112"},
{2200000, "2.2MΩ ±1% 100mW 厚膜电阻",	"C22938"},
{2200000, "2.2MΩ ±1% 125mW 厚膜电阻",	"C26113"},
{3000000, "3MΩ ±1% 100mW 厚膜电阻",	"C23156"},
{4700000, "4.7MΩ ±1% 100mW 厚膜电阻",	"C23163"},
{5100000, "5.1MΩ ±1% 100mW 厚膜电阻",	"C13320"},
{10000000, "10MΩ ±1% 100mW 厚膜电阻",	"C7250"},
{10000000, "10MΩ ±1% 125mW 厚膜电阻",	"C26108"},
{10000000, "10MΩ ±1% 62.5mW 厚膜电阻",	"C26082"},
{100000000, "100mΩ ±1% 250mW 厚膜电阻",	"C25334"},
};

template <typename T, unsigned S>
constexpr unsigned array_size(T(&)[S])
{
    return S;
} 

Resistance R_lib::operator()(int idx) const
{
    switch (current_type)
    {
        case regular_R:
            if (idx < array_size(E24_R))
                return E24_R[idx];
            else
                throw std::out_of_range("range out");
        case good_R:
            if (idx < array_size(E96_R))
                return E96_R[idx];
            else
                throw std::out_of_range("range out");
        case free_R:
            if (idx < array_size(JLC_free))
                return JLC_free[idx];
            else
                throw std::out_of_range("range out");
    }
    return 0;
}
