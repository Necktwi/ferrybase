#ifndef METAPHONE3_H
#define METAPHONE3_H

#include <string>
#include <vector>
#include <utility> // For std::pair
#include <unicode/uchar.h> // ICU for UChar32 and Unicode functions

class Metaphone3Encoder {
public:
    // Configuration options
    bool EncodeVowels = false;
    bool EncodeExact = false;
    int MaxLength = 8; // Default value

    // Constructor
    Metaphone3Encoder(bool encodeVowels = false, bool encodeExact = false, int maxLength = 8);

    // Main encoding function
    std::pair<std::string, std::string> encode(const std::string& input);

private:
    // Internal state (reset per Encode call)
    std::vector<UChar32> in;
    int idx = 0;
    int lastIdx = 0;
    std::vector<UChar32> primBuf;
    std::vector<UChar32> secondBuf;
    bool flagAlInversion = false;
    static const int DefaultMaxLength = 8;
    static const UChar32 REPLACEMENT_CHAR = 0xFFFD; // Unicode Replacement Character

    // Internal helper methods corresponding to Go funcs
    void resetState();
    void primeBuf(std::vector<UChar32>& buf, int ensureCap);
    std::string u32VectorToString(const std::vector<UChar32>& u32vec);
    std::vector<UChar32> stringToU32Vector(const std::string& utf8Str);

    void metaphAdd(UChar32 primary);
    void metaphAddAlt(UChar32 primary, UChar32 secondary);
    void metaphAddStr(const std::vector<UChar32>& primary, const std::vector<UChar32>& secondary);
    void metaphAddExactApprox(const std::vector<UChar32>& exact, const std::vector<UChar32>& main);
    void metaphAddExactApproxAlt(const std::vector<UChar32>& exact, const std::vector<UChar32>& altExact,
                                 const std::vector<UChar32>& main, const std::vector<UChar32>& alt);


    bool isVowel(UChar32 c);
    bool isVowelAt(int offset);
    bool charAt(int offset, UChar32 c);
    bool charNextIs(UChar32 c);
    bool frontVowel(int offset);

    bool stringAt(int offset, const std::vector<std::vector<UChar32>>& vals);
    bool stringAtStart(int offset, const std::vector<std::vector<UChar32>>& vals);
    bool stringAtEnd(int offset, const std::vector<std::vector<UChar32>>& vals);
    bool stringStart(const std::vector<std::vector<UChar32>>& vals);
    bool stringEnd(const std::vector<std::vector<UChar32>>& vals);
    bool stringExact(const std::vector<std::vector<UChar32>>& vals);
    bool stringContains(const std::vector<UChar32>& val);

    bool rootOrInflections(const std::vector<UChar32>& root);
    bool isSlavoGermanic();

    int skipVowels(int currentIdx);
    void advanceCounter(int noEncodeVowel, int encodeVowel);

    // --- Encoding functions for each letter ---
    void encodeB();
    bool encodeSilentB();
    void encodeC();
    bool encodeSilentCAtBeginning();
    bool encodeCaToS();
    bool encodeCoToS();
    bool encodeCh();
    bool encodeChae();
    bool encodeChToH();
    bool encodeSilentCh();
    bool encodeChToX();
    bool encodeEnglishChToK();
    bool encodeGermanicChToK();
    bool encodeArch();
    bool encodeGreekChInitial();
    bool encodeGreekChNonInitial();
    bool encodeCcia();
    bool encodeCc();
    bool encodeCkCgCq();
    bool encodeCFrontVowel();
    bool encodeBritishSilentCE();
    bool encodeCe();
    bool encodeCi();
    bool encodeLatinateSuffixes();
    bool encodeSilentC();
    bool encodeCz();
    bool encodeCs();
    void encodeD();
    bool encodeDg();
    bool encodeDj();
    bool encodeDtDd();
    bool encodeDToJ();
    bool encodeDous();
    bool encodeSilentD();
    void encodeF();
    void encodeG();
    bool encodeSilentGAtBeginning();
    bool encodeGg();
    bool encodeGk();
    bool encodeGh();
    bool encodeGhAfterConsonant();
    bool encodeInitialGh();
    bool encodeGhToJ();
    bool encodeGhToH();
    bool encodeUght();
    bool encodeGhHPartOfOtherWord();
    bool encodeSilentGh();
    bool encodeGhSpecialCases();
    bool encodeGhToF();
    bool encodeSilentG();
    bool encodeGn();
    bool encodeGl();
    bool encodeInitialGFrontVowel();
    bool initialGSoft();
    bool encodeNger();
    bool encodeGer();
    bool encodeGel();
    bool encodeNonInitialGFrontVowel();
    bool internalHardG();
    bool internalHardNg();
    bool internalHardGenGinGetGit();
    bool internalHardGOpenSyllable();
    bool internalHardGOther();
    bool encodeGaToJ();
    void encodeH();
    bool encodeInitialSilentH();
    bool encodeInitialHs();
    bool encodeInitialHuHw();
    bool encodeNonInitialSilentH();
    bool encodeHPronounced();
    void encodeJ();
    bool encodeSpanishJ();
    bool encodeGermanJ();
    bool encodeSpanishOjUj();
    bool encodeJToJ();
    bool encodeSpanishJ2();
    bool encodeJAsVowel();
    bool namesBeginningWithJThatGetAltY();
    void encodeK();
    bool encodeSilentK();
    void encodeL();
    void interpolateVowelWhenConsLAtEnd();
    bool encodeLelyToL();
    bool encodeColonel();
    bool encodeFrenchAult();
    bool encodeFrenchEuil();
    bool encodeFrenchOulx();
    bool encodeSilentLInLm();
    bool encodeSilentLInLkLv();
    bool encodeSilentLInOuld();
    bool encodeLlAsVowelSpecialCases();
    bool encodeLlAsVowel();
    bool encodeLlAsVowelCases();
    bool encodeVowelLeTransposition(int savedIdx);
    bool encodeVowelPreserveVowelAfterL(int savedIdx);
    void encodeLeCases(int savedIdx);
    void encodeM();
    bool encodeSilentMAtBeginning();
    bool encodeMrAndMrs();
    bool encodeMac();
    bool encodeMpt();
    void encodeMb();
    bool testSilentMb1();
    bool testPronouncedMb();
    bool testSilentMb2();
    bool testPronouncedMb2();
    bool testMn();
    void encodeN();
    bool encodeNce();
    void encodeP();
    bool encodeSilentPAtBeginning();
    bool encodePt();
    bool encodePh();
    bool encodePph();
    bool encodeRps();
    bool encodeCoup();
    bool encodePneum();
    bool encodePsych();
    bool encodePsalm();
    void encodePb();
    void encodeQ();
    void encodeR();
    bool encodeRz();
    bool testSilentR();
    bool encodeVowelReTransposition();
    void encodeS();
    bool encodeSkj();
    bool encodeSpecialSw();
    bool namesBeginningWithSwThatGetAltSv();
    bool namesBeginningWithSwThatGetAlvXV();
    bool encodeSj();
    bool encodeSilentFrenchSFinal();
    bool encodeSilentFrenchSInternal();
    bool encodeIsl();
    bool encodeStl();
    bool encodeChristmas();
    bool encodeSthm();
    bool encodeIsten();
    bool encodeSugar();
    bool encodeSh();
    bool encodeSch();
    bool encodeSur();
    bool encodeSu();
    bool encodeSsio();
    bool encodeSs();
    bool encodeSia();
    bool encodeSio();
    bool encodeAnglicisations();
    bool encodeSc();
    bool encodeSeiSuiSier();
    bool encodeSea();
    void encodeT();
    bool encodeTInitial();
    bool encodeTch();
    bool encodeSilentFrenchT();
    bool encodeTunTulTuaTuo();
    bool encodeTueTeuTeouTulTie();
    bool encodeTurTiuSuffixes();
    bool encodeTi();
    bool encodeTient();
    bool encodeTsch();
    bool encodeTzsch();
    bool encodeThPronouncedSeparately();
    bool encodeTth();
    bool encodeTh();
    void encodeV();
    void encodeW();
    bool encodeSilentWAtBeginning();
    bool encodeWitzWicz();
    bool encodeWr();
    bool encodeInitialWVowel();
    bool encodeWh();
    bool encodeEasternEuropeanW();
    bool germanicOrSlavicNameBeginningWithW();
    void encodeX();
    bool encodeInitialX();
    bool encodeGreekX();
    bool encodeXSpecialCases();
    bool encodeXToH();
    bool encodeXVowel();
    bool encodeFrenchXFinal();
    void encodeZ();
    bool encodeZz();
    bool encodeZuZierZs();
    bool encodeFrenchEz();
    bool encodeGermanZ();
    bool encodeZh();
    void encodeVowels();
    bool encodeSkipSilentUe();
    void encodeEPronounced();
    bool encodeOSilent();
    bool encodeESilent();
    bool encodeEPronouncedAtEnd();
    bool encodeSilentInternalE();
    bool encodeESuffix(int at);
    bool encodeEPronouncedExceptions();

    // Helper to convert string literals to vector<UChar32> for comparisons
    static std::vector<UChar32> L(const char* s);
    static std::vector<std::vector<UChar32>> LL(const std::vector<const char*>& v);
};


// Helper function (can be outside class or static)
bool areEqual(const std::vector<UChar32>& v1, const std::vector<UChar32>& v2);


#endif // METAPHONE3_H
