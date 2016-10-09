#include <stdio.h>
#include <stdint.h>
#include "pdp6common.h"

word fw(hword l, hword r) { return ((word)l << 18) | (word)r; }
hword left(word w) { return (w >> 18) & 0777777; }
hword right(word w) { return w & 0777777; }

/* just a subset here */
enum ItemType
{
	Nothing = 0,
	Code    = 1,
	Symbols = 2,
	Entry   = 4,
	End     = 5,
	Name    = 6,
	Start   = 7,
};

enum SymType
{
	SymName    = 000,
	SymGlobal  = 004,
	SymLocal   = 010,
	SymBlock   = 014,
	SymGlobalH = 044,	/* hidden
	SymLocalH  = 050,	 */
	SymUndef   = 060,
};

word item[01000000];
hword itemp, headp;
hword itemtype, itemsz;

hword relocp;
word blockreloc;
int blocksz;

void
startblock(void)
{
	if(blocksz){
		blockreloc <<= 2*(18-blocksz);
		item[relocp] = blockreloc;
		relocp = itemp++;
	}
	blockreloc = 0;
	blocksz = 0;
}

void
putword(word w, int reloc)
{
	item[itemp++] = w;
	blockreloc = blockreloc<<2 | reloc;
	blocksz++;
	itemsz++;
	if(blocksz == 18)
		startblock();
}

void
startitem(hword it)
{
	if(itemsz){
		item[headp] = fw(itemtype, itemsz);
		headp = itemp++;
	}
	itemtype = it;
	itemsz = 0;
	startblock();
}

void
dump(void)
{
	hword i;
	for(i = 0; i < itemp-2; i++)
		printf("%06o: %012lo\n", i, item[i]);
}

void
dumpfile(FILE *f)
{
	hword i;
	word w;
	int j;
	for(i = 0; i < itemp-2; i++){
		w = item[i] & 0777777777777;
		for(j = 5; j >= 0; j--)
			putc((w>>j*6)&077, f);
	}
}

int
main()
{
	/* init for first item */
	headp = itemp++;
	relocp = itemp++;

	startitem(Name);
	putword(rad50(0, "PROG"), 0);

	startitem(Code);
	putword(fw(0, 0100), 1);
	putword(fw(0000000, 0000000), 0);
	putword(fw(0515240, 0254000), 0);
	putword(fw(0201100, 0000000), 0);
	putword(fw(0554302, 0000000), 0);
	putword(fw(0540242, 0000103), 1);
	putword(fw(0200046, 0000001), 0);
	putword(fw(0202045, 0000001), 0);
	putword(fw(0202246, 0000001), 0);
	putword(fw(0365100, 0000103), 1);

	putword(fw(0402000, 0000005), 0);
	putword(fw(0700200, 0010000), 0);
	putword(fw(0271240, 0002000), 0);
	putword(fw(0200105, 0000000), 0);
	putword(fw(0700340, 0010000), 0);
	putword(fw(0254000, 0000113), 1);
	putword(fw(0275240, 0000001), 0);
	putword(fw(0202240, 0000037), 0);
	putword(fw(0275240, 0000177), 0);
	putword(fw(0574100, 0000000), 0);
	putword(fw(0322100, 0000143), 1);
	putword(fw(0213000, 0000002), 0);
	putword(fw(0550040, 0000122), 1);
	putword(fw(0271042, 0000000), 0);
	putword(fw(0504040, 0000002), 0);
	putword(fw(0275241, 0000001), 0);
	putword(fw(0542240, 0000140), 1);
	putword(fw(0200100, 0000125), 1);
	putword(fw(0271105, 0000001), 0);
	putword(fw(0202100, 0000132), 1);
	putword(fw(0201100, 0000134), 1);
	putword(fw(0202100, 0000036), 0);
	putword(fw(0200101, 0777777), 0);
	putword(fw(0202101, 0000140), 1);
	putword(fw(0274040, 0000551), 1);
	putword(fw(0325040, 0000137), 1);

	putword(fw(0200040, 0000552), 1);
	putword(fw(0251040, 0000057), 0);
	putword(fw(0200040, 0000147), 1);
	putword(fw(0202040, 0000101), 1);
	putword(fw(0254020, 0000100), 1);
	putword(fw(0000000, 0000000), 0);
	putword(fw(0264000, 0000000), 0);
	putword(fw(0264000, 0000000), 0);
	putword(fw(0265240, 0000000), 0);
	putword(fw(0264000, 0000000), 0);
	putword(fw(0265240, 0000153), 1);
	putword(fw(0264000, 0000000), 0);
	putword(fw(0265240, 0000155), 1);
	putword(fw(0264000, 0000000), 0);
	putword(fw(0265240, 0000157), 1);
	putword(fw(0264000, 0000000), 0);
	putword(fw(0265240, 0000161), 1);
	putword(fw(0264000, 0000000), 0);
	putword(fw(0265240, 0000163), 1);
	putword(fw(0264000, 0000000), 0);
	putword(fw(0265240, 0000165), 1);

	putword(fw(0000000, 0000000), 0);
	putword(fw(0201040, 0777777), 1);
	putword(fw(0202040, 0000000), 0);
	putword(fw(0200240, 0000443), 1);
	putword(fw(0200140, 0000553), 1);
	putword(fw(0260140, 0000000), 0);
	putword(fw(0201040, 0000000), 0);
	putword(fw(0260140, 0000000), 0);
	putword(fw(0201040, 0000474), 1);
	putword(fw(0260140, 0000177), 1);
	putword(fw(0554040, 0000000), 0);
	putword(fw(0260140, 0000000), 0);
	putword(fw(0201040, 0000554), 1);
	putword(fw(0260140, 0000201), 1);
	putword(fw(0550040, 0000202), 1);
	putword(fw(0260140, 0000203), 1);
	putword(fw(0201040, 0000555), 1);
	putword(fw(0260140, 0000205), 1);
	putword(fw(0201040, 0000000), 0);
	putword(fw(0260140, 0000211), 1);
	putword(fw(0260140, 0000175), 1);
	putword(fw(0260140, 0000421), 1);

	putword(fw(0402000, 0000000), 0);
	putword(fw(0200240, 0000443), 1);
	putword(fw(0201040, 0000477), 1);
	putword(fw(0260140, 0000213), 1);
	putword(fw(0260140, 0000421), 1);
	putword(fw(0260140, 0000376), 1);
	putword(fw(0254000, 0000216), 1);
	putword(fw(0260140, 0000000), 0);
	putword(fw(0254000, 0000216), 1);
	putword(fw(0332000, 0000002), 0);
	putword(fw(0303100, 0000014), 0);
	putword(fw(0254000, 0000216), 1);
	putword(fw(0275100, 0000001), 0);
	putword(fw(0221100, 0000037), 0);
	putword(fw(0272100, 0000216), 1);
	putword(fw(0260140, 0000225), 1);
	putword(fw(0254000, 0000216), 1);
	putword(fw(0332000, 0000002), 0);
	putword(fw(0303100, 0000037), 0);
	putword(fw(0254000, 0000216), 1);
	putword(fw(0275100, 0000001), 0);
	putword(fw(0272100, 0000234), 1);
	putword(fw(0260140, 0000235), 1);
	putword(fw(0254000, 0000216), 1);
	putword(fw(0301100, 0000101), 0);
	putword(fw(0303100, 0000143), 0);
	putword(fw(0254000, 0000216), 1);
	putword(fw(0275100, 0000100), 0);
	putword(fw(0221100, 0000564), 0);
	putword(fw(0272100, 0000243), 1);

	putword(fw(0402000, 0000000), 0);
	putword(fw(0200240, 0000443), 1);
	putword(fw(0201040, 0000506), 1);
	putword(fw(0260140, 0000221), 1);
	putword(fw(0260140, 0000421), 1);
	putword(fw(0260140, 0000376), 1);
	putword(fw(0254000, 0000254), 1);
	putword(fw(0134100, 0000001), 0);
	putword(fw(0275100, 0000060), 0);
	putword(fw(0331000, 0000002), 0);
	putword(fw(0303100, 0000002), 0);
	putword(fw(0254000, 0000254), 1);
	putword(fw(0220100, 0000556), 1);
	putword(fw(0272100, 0000254), 1);
	putword(fw(0134100, 0000001), 0);
	putword(fw(0275100, 0000060), 0);
	putword(fw(0331000, 0000002), 0);
	putword(fw(0303100, 0000011), 0);
	putword(fw(0254000, 0000254), 1);
	putword(fw(0221100, 0645700), 0);
	putword(fw(0272100, 0000271), 1);
	putword(fw(0260140, 0000244), 1);
	putword(fw(0254000, 0000254), 1);
	putword(fw(0303100, 0000073), 0);
	putword(fw(0254000, 0000254), 1);
	putword(fw(0221100, 0007020), 0);
	putword(fw(0272100, 0000300), 1);

	putword(fw(0200240, 0000443), 1);
	putword(fw(0201040, 0000517), 1);
	putword(fw(0260140, 0000257), 1);
	putword(fw(0554300, 0000000), 0);
	putword(fw(0201040, 0000004), 0);
	putword(fw(0261140, 0000001), 0);
	putword(fw(0201040, 0000001), 0);
	putword(fw(0510206, 0000000), 0);
	putword(fw(0554306, 0000003), 0);
	putword(fw(0322300, 0000324), 1);
	putword(fw(0510106, 0000000), 0);
	putword(fw(0316100, 0000004), 0);
	putword(fw(0344040, 0000317), 1);
	putword(fw(0200340, 0000001), 0);
	putword(fw(0260140, 0000207), 1);
	putword(fw(0201600, 0000040), 0);
	putword(fw(0136600, 0000005), 0);
	putword(fw(0201600, 0000003), 0);
	putword(fw(0200440, 0000557), 1);
	putword(fw(0134100, 0000011), 0);
	putword(fw(0271100, 0000240), 0);
	putword(fw(0136100, 0000005), 0);
	putword(fw(0367600, 0000332), 1);
	putword(fw(0201040, 0000560), 1);
	putword(fw(0303340, 0000001), 0);
	putword(fw(0260140, 0000311), 1);
	putword(fw(0260140, 0000214), 1);
	putword(fw(0326300, 0000315), 1);
	putword(fw(0262140, 0000002), 0);
	putword(fw(0260140, 0000421), 1);

	putword(fw(0200240, 0000443), 1);
	putword(fw(0201040, 0000524), 1);
	putword(fw(0260140, 0000340), 1);
	putword(fw(0260140, 0000421), 1);
	putword(fw(0260140, 0000376), 1);
	putword(fw(0254000, 0000355), 1);
	putword(fw(0201040, 0000000), 0);
	putword(fw(0202040, 0000172), 1);
	putword(fw(0200240, 0000443), 1);
	putword(fw(0201040, 0000536), 1);
	putword(fw(0260140, 0000347), 1);
	putword(fw(0260140, 0000421), 1);
	putword(fw(0260140, 0000376), 1);
	putword(fw(0254000, 0000365), 1);
	putword(fw(0200040, 0000000), 0);
	putword(fw(0552040, 0000354), 1);

	putword(fw(0200240, 0000443), 1);
	putword(fw(0201040, 0000541), 1);
	putword(fw(0260140, 0000357), 1);
	putword(fw(0200040, 0000364), 1);
	putword(fw(0260140, 0000000), 0);
	putword(fw(0201040, 0000544), 1);
	putword(fw(0260140, 0000367), 1);
	putword(fw(0260140, 0000421), 1);
	putword(fw(0254020, 0000170), 1);

	putword(fw(0200040, 0000443), 1);
	putword(fw(0201200, 0000000), 0);
	putword(fw(0712340, 0000040), 0);
	putword(fw(0254000, 0000400), 1);
	putword(fw(0712040, 0000002), 0);
	putword(fw(0712140, 0000002), 0);
	putword(fw(0136100, 0000001), 0);
	putword(fw(0405100, 0000177), 0);
	putword(fw(0306100, 0000177), 0);
	putword(fw(0254000, 0000000), 0);
	putword(fw(0302100, 0000015), 0);
	putword(fw(0344200, 0000400), 1);
	putword(fw(0712340, 0000020), 0);
	putword(fw(0254000, 0000412), 1);
	putword(fw(0201100, 0000012), 0);
	putword(fw(0712140, 0000002), 0);
	putword(fw(0200040, 0000443), 1);
	putword(fw(0326200, 0000000), 0);
	putword(fw(0263140, 0000000), 0);

	putword(fw(0200040, 0000443), 1);
	putword(fw(0712040, 0000002), 0);
	putword(fw(0316040, 0000005), 0);
	putword(fw(0263140, 0000000), 0);
	putword(fw(0712300, 0000040), 0);
	putword(fw(0254000, 0000434), 1);
	putword(fw(0712300, 0000020), 0);
	putword(fw(0254000, 0000427), 1);
	putword(fw(0134100, 0000001), 0);
	putword(fw(0712140, 0000002), 0);
	putword(fw(0254000, 0000423), 1);
	putword(fw(0712300, 0000020), 0);
	putword(fw(0254000, 0000434), 1);
	putword(fw(0712140, 0000561), 1);
	putword(fw(0712300, 0000020), 0);
	putword(fw(0254000, 0000437), 1);
	putword(fw(0712140, 0000562), 1);
	putword(fw(0263140, 0000000), 0);
	putword(fw(0440700, 0000444), 1);

	startitem(Code);
	putword(fw(0, 0474), 1);
	putword(fw(0202505, 0620246), 0);
	putword(fw(0271010, 0554212), 0);
	putword(fw(0415320, 0000000), 0);
	putword(fw(0064252, 0454640), 0);
	putword(fw(0425012, 0447610), 0);
	putword(fw(0406624, 0751500), 0);
	putword(fw(0422032, 0442500), 0);
	putword(fw(0406464, 0040604), 0);
	putword(fw(0476550, 0527032), 0);
	putword(fw(0050000, 0000000), 0);
	putword(fw(0064252, 0454640), 0);
	putword(fw(0425006, 0420210), 0);
	putword(fw(0446171, 0152100), 0);
	putword(fw(0472032, 0654500), 0);
	putword(fw(0522231, 0542520), 0);
	putword(fw(0301626, 0531500), 0);
	putword(fw(0446464, 0034564), 0);
	putword(fw(0325464, 0040632), 0);
	putword(fw(0245341, 0505000), 0);
	putword(fw(0064251, 0147500), 0);
	putword(fw(0416371, 0643222), 0);
	putword(fw(0436532, 0240650), 0);
	putword(fw(0446371, 0606424), 0);
	putword(fw(0000000, 0000000), 0);
	putword(fw(0064250, 0447500), 0);
	putword(fw(0546372, 0520256), 0);
	putword(fw(0406352, 0420246), 0);
	putword(fw(0546471, 0540626), 0);
	putword(fw(0242513, 0150212), 0);
	putword(fw(0202624, 0044614), 0);
	putword(fw(0202630, 0551530), 0);
	putword(fw(0416444, 0044614), 0);
	putword(fw(0202351, 0724576), 0);
	putword(fw(0064240, 0000000), 0);
	putword(fw(0064250, 0554212), 0);
	putword(fw(0415010, 0442250), 0);
	putword(fw(0374321, 0200000), 0);
	putword(fw(0064250, 0554212), 0);
	putword(fw(0415011, 0151500), 0);
	putword(fw(0000000, 0000000), 0);
	putword(fw(0202370, 0352202), 0);
	putword(fw(0461011, 0447606), 0);
	putword(fw(0406511, 0147634), 0);
	putword(fw(0515011, 0447634), 0);
	putword(fw(0435341, 0505000), 0);
	putword(fw(0000001, 0000001), 0);
	putword(fw(0000150, 0000040), 2);

	putword(fw(0000010, 0000464), 1);
	putword(fw(0270000, 0000000), 0);
	putword(fw(0200000, 0000000), 0);
	putword(fw(0000000, 0106274), 0);
	putword(fw(0440600, 0000004), 0);
	putword(fw(0236460, 0000000), 0);
	putword(fw(0000000, 0000015), 0);
	putword(fw(0000000, 0000012), 0);

	startitem(Symbols);
	putword(rad50(SymGlobal, "A"), 0);
	putword(0000000, 0);
	putword(rad50(SymGlobal, "AC1"), 0);
	putword(0000015, 0);
	putword(rad50(SymGlobal, "AC2"), 0);
	putword(0000016, 0);
	putword(rad50(SymGlobal, "AC3"), 0);
	putword(0000017, 0);
	putword(rad50(SymGlobal, "AL"), 0);
	putword(0000001, 0);
	putword(rad50(SymGlobal, "ASSCON"), 0);
	putword(0400000, 0);
	putword(rad50(SymGlobal, "ASSPRG"), 0);
	putword(0200000, 0);
	putword(rad50(SymGlobal, "B"), 0);
	putword(0000014, 0);
	putword(rad50(SymGlobal, "BUFPNT"), 0);
	putword(0000012, 0);
	putword(rad50(SymGlobal, "BUFWRD"), 0);
	putword(0000013, 0);
	putword(rad50(SymLocal, "C"), 0);
	putword(0000002, 0);
	putword(rad50(SymUndef, "CH1"), 0);
	putword(0000152, 1);
	putword(rad50(SymUndef, "CH2"), 0);
	putword(0000154, 1);
	putword(rad50(SymUndef, "CH3"), 0);
	putword(0000156, 1);
	putword(rad50(SymUndef, "CH4"), 0);
	putword(0000160, 1);
	putword(rad50(SymUndef, "CH5"), 0);
	putword(0000162, 1);
	putword(rad50(SymUndef, "CH6"), 0);
	putword(0000164, 1);
	putword(rad50(SymUndef, "CH7"), 0);
	putword(0000166, 1);
	putword(rad50(SymLocal, "CHL"), 0);
	putword(0000006, 0);
	putword(rad50(SymGlobal, "CLOSR"), 0);
	putword(0002000, 0);
	putword(rad50(SymGlobal, "CLSIN"), 0);
	putword(0000002, 0);
	putword(rad50(SymGlobal, "CLSOUT"), 0);
	putword(0000001, 0);
	putword(rad50(SymUndef, "CONFIG"), 0);
	putword(0000176, 1);
	putword(rad50(SymUndef, "CONMES"), 0);
	putword(0000373, 1);
	putword(rad50(SymUndef, "CPOPJ"), 0);
	putword(0000407, 1);
	putword(rad50(SymUndef, "CPOPJ1"), 0);
	putword(0000417, 1);
	putword(rad50(SymUndef, "CRLF"), 0);
	putword(0000341, 1);
	putword(rad50(SymGlobal, "D"), 0);
	putword(0000017, 0);
	putword(rad50(SymGlobal, "DAT"), 0);
	putword(0000005, 0);
	putword(rad50(SymLocal, "DATLOP"), 0);
	putword(0000216, 1);
	putword(rad50(SymGlobal, "DCL"), 0);
	putword(0000001, 0);
	putword(rad50(SymGlobal, "DCW"), 0);
	putword(0020000, 0);
	putword(rad50(SymGlobal, "DDI"), 0);
	putword(0000007, 0);
	putword(rad50(SymGlobal, "DDO"), 0);
	putword(0000006, 0);
	putword(rad50(SymGlobal, "DDTMEM"), 0);
	putword(0000037, 0);
	putword(rad50(SymGlobal, "DDTSYM"), 0);
	putword(0000036, 0);
	putword(rad50(SymUndef, "DECIN"), 0);
	putword(0000000, 0);
	putword(rad50(SymUndef, "DECIN1"), 0);
	putword(0000301, 1);
	putword(rad50(SymGlobal, "DEN"), 0);
	putword(0000004, 0);
	putword(rad50(SymLocal, "DEV1"), 0);
	putword(0000103, 1);
	putword(rad50(SymGlobal, "DEVADR"), 0);
	putword(0000007, 0);
	putword(rad50(SymGlobal, "DEVBUF"), 0);
	putword(0000006, 0);
	putword(rad50(SymGlobal, "DEVCHR"), 0);
	putword(0000001, 0);
	putword(rad50(SymGlobal, "DEVCTR"), 0);
	putword(0000011, 0);
	putword(rad50(SymGlobal, "DEVDAT"), 0);
	putword(0000006, 0);
	putword(rad50(SymGlobal, "DEVIAD"), 0);
	putword(0000007, 0);
	putword(rad50(SymUndef, "DECINT"), 0);
	putword(0000104, 1);
	putword(rad50(SymGlobal, "DEVIOS"), 0);
	putword(0000002, 0);
	putword(rad50(SymGlobal, "DEVLOG"), 0);
	putword(0000005, 0);
	putword(rad50(SymUndef, "DEVLST"), 0);
	putword(0000312, 1);
	putword(rad50(SymGlobal, "DEVMOD"), 0);
	putword(0000004, 0);
	putword(rad50(SymGlobal, "DEVNAM"), 0);
	putword(0000000, 0);
	putword(rad50(SymGlobal, "DEVOAD"), 0);
	putword(0000010, 0);
	putword(rad50(SymGlobal, "DEVPTR"), 0);
	putword(0000010, 0);
	putword(rad50(SymGlobal, "DEVSER"), 0);
	putword(0000003, 0);
	putword(rad50(SymGlobal, "DGF"), 0);
	putword(0000012, 0);
	putword(rad50(SymGlobal, "DIN"), 0);
	putword(0000003, 0);
	putword(rad50(SymGlobal, "DLK"), 0);
	putword(0000005, 0);
	putword(rad50(SymGlobal, "DOU"), 0);
	putword(0000002, 0);
	putword(rad50(SymGlobal, "DR"), 0);
	putword(0000016, 0);
	putword(rad50(SymGlobal, "DRL"), 0);
	putword(0000000, 0);
	putword(rad50(SymGlobal, "DSI"), 0);
	putword(0000011, 0);
	putword(rad50(SymGlobal, "DSO"), 0);
	putword(0000010, 0);
	putword(rad50(SymGlobal, "DTW"), 0);
	putword(0040000, 0);
	putword(rad50(SymGlobal, "DVAVAL"), 0);
	putword(0000040, 0);
	putword(rad50(SymGlobal, "DVCDR"), 0);
	putword(0100000, 0);
	putword(rad50(SymGlobal, "DVDIR"), 0);
	putword(0000004, 0);
	putword(rad50(SymGlobal, "DVDIR1"), 0);
	putword(0400000, 0);
	putword(rad50(SymGlobal, "DVIN"), 0);
	putword(0000002, 0);
	putword(rad50(SymGlobal, "DVLPT"), 0);
	putword(0040000, 0);
	putword(rad50(SymGlobal, "DVMTA"), 0);
	putword(0000020, 0);
	putword(rad50(SymGlobal, "DVOUT"), 0);
	putword(0000001, 0);
	putword(rad50(SymGlobal, "DVTTY"), 0);
	putword(0000010, 0);
	putword(rad50(SymGlobal, "ENTRB"), 0);
	putword(0020000, 0);
	putword(rad50(SymUndef, "ERNAM"), 0);
	putword(0000000, 0);
	putword(rad50(SymUndef, "ERROR"), 0);
	putword(0000167, 1);
	putword(rad50(SymLocal, "EXECIS"), 0);
	putword(0000541, 1);
	putword(rad50(SymLocal, "GET1"), 0);
	putword(0000400, 1);
	putword(rad50(SymLocal, "GETLIN"), 0);
	putword(0000376, 1);
	putword(rad50(SymGlobal, "I"), 0);
	putword(0000010, 0);
	putword(rad50(SymGlobal, "IB"), 0);
	putword(0000013, 0);
	putword(rad50(SymGlobal, "IBUFB"), 0);
	putword(0200000, 0);
	putword(rad50(SymGlobal, "INITB"), 0);
	putword(0400000, 0);
	putword(rad50(SymGlobal, "INPB"), 0);
	putword(0010000, 0);
	putword(rad50(SymLocal, "INT"), 0);
	putword(0000005, 0);
	putword(rad50(SymGlobal, "IO"), 0);
	putword(0000020, 0);
	putword(rad50(SymGlobal, "IOACT"), 0);
	putword(0010000, 0);
	putword(rad50(SymGlobal, "IOREG"), 0);
	putword(0000002, 0);
	putword(rad50(SymGlobal, "IORKTL"), 0);
	putword(0040000, 0);
	putword(rad50(SymGlobal, "IOCON"), 0);
	putword(0000040, 0);
	putword(rad50(SymLocal, "IOCONF"), 0);
	putword(0000517, 1);
	putword(rad50(SymGlobal, "IODEND"), 0);
	putword(0020000, 0);
	putword(rad50(SymGlobal, "IODERR"), 0);
	putword(0200000, 0);
	putword(rad50(SymGlobal, "IODISC"), 0);
	putword(0400000, 0);
	putword(rad50(SymGlobal, "IODONE"), 0);
	putword(0400000, 0);
	putword(rad50(SymGlobal, "IODTER"), 0);
	putword(0100000, 0);
	putword(rad50(SymGlobal, "IOEND"), 0);
	putword(0000040, 0);
	putword(rad50(SymGlobal, "IOFST"), 0);
	putword(0000004, 0);
	putword(rad50(SymGlobal, "IOIMPM"), 0);
	putword(0400000, 0);
	putword(rad50(SymGlobal, "IONRCK"), 0);
	putword(0000100, 0);
	putword(rad50(SymGlobal, "IORDEL"), 0);
	putword(0000100, 0);
	putword(rad50(SymGlobal, "IORET"), 0);
	putword(0000020, 0);
	putword(rad50(SymGlobal, "IOS"), 0);
	putword(0000000, 0);
	putword(rad50(SymGlobal, "IOSTRT"), 0);
	putword(0000010, 0);
	putword(rad50(SymGlobal, "IOUSE"), 0);
	putword(0400000, 0);
	putword(rad50(SymGlobal, "IOW"), 0);
	putword(0000001, 0);
	putword(rad50(SymGlobal, "IOWC"), 0);
	putword(0000020, 0);
	putword(rad50(SymGlobal, "IOWS"), 0);
	putword(0400000, 0);
	putword(rad50(SymGlobal, "ITEM"), 0);
	putword(0000004, 0);
	putword(rad50(SymGlobal, "JBFADR"), 0);
	putword(0000000, 0);
	putword(rad50(SymGlobal, "JBFCTR"), 0);
	putword(0000002, 0);
	putword(rad50(SymGlobal, "JBFPTR"), 0);
	putword(0000001, 0);
	putword(rad50(SymGlobal, "JBUF"), 0);
	putword(0000005, 0);
	putword(rad50(SymGlobal, "JDAT"), 0);
	putword(0000011, 0);
	putword(rad50(SymGlobal, "JERR"), 0);
	putword(0002000, 0);
	putword(rad50(SymGlobal, "JIOW"), 0);
	putword(0100000, 0);
	putword(rad50(SymGlobal, "JNA"), 0);
	putword(0004000, 0);
	putword(rad50(SymUndef, "JORFF"), 0);
	putword(0000363, 1);
	putword(rad50(SymUndef, "JORSYM"), 0);
	putword(0000135, 1);
	putword(rad50(SymLocal, "JRSTI"), 0);
	putword(0000147, 1);
	putword(rad50(SymLocal, "JRSTI1"), 0);
	putword(0000143, 1);
	putword(rad50(SymLocal, "LENGTH"), 0);
	putword(0000544, 1);
	putword(rad50(SymLocal, "LINBUF"), 0);
	putword(0000444, 1);
	putword(rad50(SymLocal, "LINEP"), 0);
	putword(0000443, 1);
	putword(rad50(SymGlobal, "LINKSR"), 0);
	putword(0000100, 1);
	putword(rad50(SymLocal, "LOC40"), 0);
	putword(0000150, 1);
	putword(rad50(SymGlobal, "LOOKB"), 0);
	putword(0040000, 0);
	putword(rad50(SymUndef, "MAKEND"), 0);
	putword(0000353, 1);
	putword(rad50(SymGlobal, "MTW"), 0);
	putword(0010000, 0);
	putword(rad50(SymUndef, "NDEVM1"), 0);
	putword(0000102, 1);
	putword(rad50(SymLocal, "NXM"), 0);
	putword(0010000, 0);
	putword(rad50(SymGlobal, "OBUFB"), 0);
	putword(0100000, 0);
	putword(rad50(SymUndef, "OCTPNT"), 0);
	putword(0000371, 1);
	putword(rad50(SymGlobal, "ONCE"), 0);
	putword(0000170, 1);
	putword(rad50(SymLocal, "ONCE2"), 0);
	putword(0000355, 1);
	putword(rad50(SymLocal, "ONCE3"), 0);
	putword(0000365, 1);
	putword(rad50(SymLocal, "ONCE4"), 0);
	putword(0000317, 1);
	putword(rad50(SymLocal, "ONCE5"), 0);
	putword(0000315, 1);
	putword(rad50(SymLocal, "ONCE6"), 0);
	putword(0000324, 1);
	putword(rad50(SymLocal, "ONCE7"), 0);
	putword(0000332, 1);
	putword(rad50(SymLocal, "ONCEPD"), 0);
	putword(0000464, 1);
	putword(rad50(SymLocal, "ONCEPN"), 0);
	putword(0000010, 0);
	putword(rad50(SymLocal, "OPOUT"), 0);
	putword(0000421, 1);
	putword(rad50(SymLocal, "OPOUT1"), 0);
	putword(0000423, 1);
	putword(rad50(SymLocal, "OPOUT2"), 0);
	putword(0000434, 1);
	putword(rad50(SymGlobal, "OUTPR"), 0);
	putword(0004000, 0);
	putword(rad50(SymGlobal, "PATCH"), 0);
	putword(0000000, 1);
	putword(rad50(SymGlobal, "PDP"), 0);
	putword(0000003, 0);
	putword(rad50(SymGlobal, "PICHN"), 0);
	putword(0000100, 0);
	putword(rad50(SymGlobal, "PROG"), 0);
	putword(0000007, 0);
	putword(rad50(SymUndef, "RADX10"), 0);
	putword(0000325, 1);
	putword(rad50(SymGlobal, "RUN"), 0);
	putword(0200000, 0);
	putword(rad50(SymGlobal, "RUNABL"), 0);
	putword(0204000, 0);
	putword(rad50(SymLocal, "STO1"), 0);
	putword(0000140, 1);
	putword(rad50(SymUndef, "SYSDAT"), 0);
	putword(0000212, 1);
	putword(rad50(SymLocal, "SYSDM"), 0);
	putword(0000536, 1);
	putword(rad50(SymGlobal, "SYSFIN"), 0);
	putword(0777777777777, 1);
	putword(rad50(SymLocal, "SYSM"), 0);
	putword(0000524, 1);
	putword(rad50(SymUndef, "SYSNUM"), 0);
	putword(0000206, 1);
	putword(rad50(SymUndef, "SYSSIZ"), 0);
	putword(0000370, 1);
	putword(rad50(SymLocal, "T"), 0);
	putword(0000001, 0);
	putword(rad50(SymGlobal, "TAC"), 0);
	putword(0000001, 0);
	putword(rad50(SymGlobal, "TAC1"), 0);
	putword(0000002, 0);
	putword(rad50(SymGlobal, "TEM"), 0);
	putword(0000010, 0);
	putword(rad50(SymUndef, "THSDAT"), 0);
	putword(0000253, 1);
	putword(rad50(SymUndef, "TIME"), 0);
	putword(0000306, 1);
	putword(rad50(SymLocal, "TIMEM"), 0);
	putword(0000506, 1);
	putword(rad50(SymLocal, "TIMLOP"), 0);
	putword(0000254, 1);
	putword(rad50(SymLocal, "TODATE"), 0);
	putword(0000477, 1);
	putword(rad50(SymLocal, "TSEXEC"), 0);
	putword(0000474, 1);
	putword(rad50(SymGlobal, "TTYATC"), 0);
	putword(0020000, 0);
	putword(rad50(SymGlobal, "TTYUSE"), 0);
	putword(0010000, 0);
	putword(rad50(SymGlobal, "USRMOD"), 0);
	putword(0010000, 0);
	putword(rad50(SymGlobal, "UUO"), 0);
	putword(0000014, 0);
	putword(rad50(SymUndef, "UUO0"), 0);
	putword(0000151, 1);

	/* Testing */

	startitem(Code);
	putword(fw(0, 0600), 1);
	putword(fw(0, 0), 0);
	putword(fw(0, 0), 0);
	putword(fw(0, 0600), 1);

	startitem(Symbols);


	putword(rad50(SymGlobal, "ERROR"), 0);
	putword(0646464, 0);
	putword(rad50(SymUndef, "ERROR"), 0);
	putword(0000602, 1);

	putword(rad50(SymGlobal, "UUO0"), 0);
	putword(04000, 0);
	putword(rad50(SymGlobal, "CH1"), 0);
	putword(04001, 0);
	putword(rad50(SymGlobal, "CH2"), 0);
	putword(04002, 0);
	putword(rad50(SymGlobal, "CH3"), 0);
	putword(04003, 0);
	putword(rad50(SymGlobal, "CH4"), 0);
	putword(04004, 0);
	putword(rad50(SymGlobal, "CH5"), 0);
	putword(04005, 0);
	putword(rad50(SymGlobal, "CH6"), 0);
	putword(04006, 0);
	putword(rad50(SymGlobal, "CH7"), 0);
	putword(04007, 0);


	startitem(Start);
	putword(0100, 0);

	startitem(End);
	putword(0, 0);
	putword(0, 0);

	startitem(Nothing);
	dump();

	{
		FILE *f;
		if(f = fopen("test.rel", "wb"), f == NULL)
			return 1;
		dumpfile(f);
		fclose(f);
	}

	return 0;
}
