enum class Abbreviation: uint16_t {
    Invalid,
    MART,
    WAKT,
    AWST,
    BORTST,
    WIB,
    MSKp3,
    HMT,
    IDDT,
    CWT,
    BNT,
    AKTST,
    TKT,
    BTT,
    MSDp4,
    CDT,
    LHDT,
    PYT,
    CDDT,
    NDT,
    ART,
    SHEST,
    HPT,
    IRDT,
    SMT,
    HWT,
    CAST,
    PHOT,
    HST,
    CST,
    JMT,
    NOVT,
    NST,
    GALT,
    STAT,
    COST,
    YWT,
    EHDT,
    WFT,
    BST,
    FRUT,
    HOVST,
    FRUST,
    VOST,
    VET,
    BRST,
    CCT,
    CHUT,
    PLMT,
    SRET,
    CEMT,
    VOLST,
    MYT,
    CET,
    AZOST,
    KIZT,
    EET,
    CHADT,
    ACDT,
    ACWST,
    ORAST,
    AWDT,
    AHST,
    KGST,
    URAST,
    MDST,
    YAKST,
    HKT,
    NMT,
    EPT,
    VUST,
    CHOT,
    UYT,
    SYOT,
    CKT,
    MUST,
    EGT,
    LST,
    PMST,
    TJT,
    FNT,
    PDDT,
    MSKp1,
    LMT,
    p10,
    YAKT,
    PWT,
    SET,
    TVT,
    WAT,
    PKT,
    YEKST,
    TAST,
    CVST,
    MSDp1,
    SVEST,
    MSKp2,
    NFST,
    AST,
    EEST,
    PMT,
    BRT,
    PET,
    TBMT,
    CGT,
    TLT,
    AKTT,
    MSM,
    YDDT,
    MST,
    CEST,
    NDDT,
    ICT,
    SAMST,
    FKST,
    WET,
    PEST,
    TOST,
    p12,
    BORT,
    HDT,
    WEMT,
    ULAST,
    NFT,
    EAST,
    YPT,
    PONT,
    ALMT,
    MMT,
    NWT,
    SAST,
    SBT,
    EGST,
    EWT,
    EDT,
    YST,
    NZST,
    KGT,
    GHST,
    WITA,
    CLST,
    GILT,
    KART,
    KRAST,
    PDT,
    DAVT,
    GEST,
    HKWT,
    ADDT,
    PPT,
    FORT,
    AKST,
    QMT,
    FKT,
    GFT,
    MAWT,
    ECT,
    ANAT,
    MSD,
    APT,
    MSK,
    TRT,
    KST,
    BURT,
    FMT,
    WAST,
    WGST,
    NPT,
    PNT,
    GST,
    SDMT,
    ISST,
    MVT,
    ACT,
    KRAT,
    ACST,
    LINT,
    SHET,
    IOT,
    CMT,
    AEST,
    ASHST,
    ULAT,
    COT,
    KUYT,
    DUSST,
    SCT,
    TFT,
    CVT,
    TMT,
    MDT,
    RMT,
    MSKp4,
    ALMST,
    PPMT,
    AZST,
    PGT,
    GYT,
    OMST,
    KDT,
    ARST,
    PKST,
    IDT,
    FFMT,
    LHST,
    NRT,
    SAKT,
    AZT,
    IRKT,
    AMST,
    NCST,
    SST,
    NOVST,
    OMSST,
    MUT,
    IMT,
    PMMT,
    PST,
    FNST,
    MDDT,
    JST,
    VOLT,
    MALST,
    ADT,
    MPT,
    SAMT,
    NUT,
    AQTT,
    CKHST,
    MAGST,
    EDDT,
    BDST,
    IST,
    HKST,
    CAT,
    HOVT,
    TAHT,
    TASST,
    WEST,
    ANAST,
    CHAST,
    WGT,
    AFT,
    NFDT,
    FJT,
    CPT,
    ORAT,
    DDUT,
    MSDp3,
    CXT,
    GMT,
    TSAT,
    AZOT,
    NZMT,
    AEDT,
    ACWDT,
    EST,
    MWT,
    SRT,
    PETT,
    ANT,
    QYZST,
    NCT,
    TOT,
    WIT,
    KMT,
    NZDT,
    RET,
    AWT,
    VLAST,
    EAT,
    KUYST,
    p09,
    SAKST,
    URUT,
    SVET,
    UYST,
    GAMT,
    VUT,
    KOST,
    CLT,
    WMT,
    p11,
    DMT,
    QYZT,
    GDT,
    MALT,
    CHOST,
    PETST,
    WST,
    BMT,
    JAVT,
    YDT,
    mm12,
    AMT,
    IRKST,
    BOT,
    EASST,
    BDT,
    JDT,
    SJMT,
    PYST,
    AHDT,
    VLAT,
    ASHT,
    GET,
    IRST,
    PMDT,
    URAT,
    DUST,
    UZT,
    TRST,
    MAGT,
    YEKT,
    EMT,
    ChST,
    AKDT
};

static constexpr const char* _AbbreviationNames[] {
    "Invalid",
    "MART",
    "WAKT",
    "AWST",
    "BORTST",
    "WIB",
    "MSK+3",
    "HMT",
    "IDDT",
    "CWT",
    "BNT",
    "AKTST",
    "TKT",
    "BTT",
    "MSD+4",
    "CDT",
    "LHDT",
    "PYT",
    "CDDT",
    "NDT",
    "ART",
    "SHEST",
    "HPT",
    "IRDT",
    "SMT",
    "HWT",
    "CAST",
    "PHOT",
    "HST",
    "CST",
    "JMT",
    "NOVT",
    "NST",
    "GALT",
    "STAT",
    "COST",
    "YWT",
    "EHDT",
    "WFT",
    "BST",
    "FRUT",
    "HOVST",
    "FRUST",
    "VOST",
    "VET",
    "BRST",
    "CCT",
    "CHUT",
    "PLMT",
    "SRET",
    "CEMT",
    "VOLST",
    "MYT",
    "CET",
    "AZOST",
    "KIZT",
    "EET",
    "CHADT",
    "ACDT",
    "ACWST",
    "ORAST",
    "AWDT",
    "AHST",
    "KGST",
    "URAST",
    "MDST",
    "YAKST",
    "HKT",
    "NMT",
    "EPT",
    "VUST",
    "CHOT",
    "UYT",
    "SYOT",
    "CKT",
    "MUST",
    "EGT",
    "LST",
    "PMST",
    "TJT",
    "FNT",
    "PDDT",
    "MSK+1",
    "LMT",
    "+10",
    "YAKT",
    "PWT",
    "SET",
    "TVT",
    "WAT",
    "PKT",
    "YEKST",
    "TAST",
    "CVST",
    "MSD+1",
    "SVEST",
    "MSK+2",
    "NFST",
    "AST",
    "EEST",
    "PMT",
    "BRT",
    "PET",
    "TBMT",
    "CGT",
    "TLT",
    "AKTT",
    "MSM",
    "YDDT",
    "MST",
    "CEST",
    "NDDT",
    "ICT",
    "SAMST",
    "FKST",
    "WET",
    "PEST",
    "TOST",
    "+12",
    "BORT",
    "HDT",
    "WEMT",
    "ULAST",
    "NFT",
    "EAST",
    "YPT",
    "PONT",
    "ALMT",
    "MMT",
    "NWT",
    "SAST",
    "SBT",
    "EGST",
    "EWT",
    "EDT",
    "YST",
    "NZST",
    "KGT",
    "GHST",
    "WITA",
    "CLST",
    "GILT",
    "KART",
    "KRAST",
    "PDT",
    "DAVT",
    "GEST",
    "HKWT",
    "ADDT",
    "PPT",
    "FORT",
    "AKST",
    "QMT",
    "FKT",
    "GFT",
    "MAWT",
    "ECT",
    "ANAT",
    "MSD",
    "APT",
    "MSK",
    "TRT",
    "KST",
    "BURT",
    "FMT",
    "WAST",
    "WGST",
    "NPT",
    "PNT",
    "GST",
    "SDMT",
    "ISST",
    "MVT",
    "ACT",
    "KRAT",
    "ACST",
    "LINT",
    "SHET",
    "IOT",
    "CMT",
    "AEST",
    "ASHST",
    "ULAT",
    "COT",
    "KUYT",
    "DUSST",
    "SCT",
    "TFT",
    "CVT",
    "TMT",
    "MDT",
    "RMT",
    "MSK+4",
    "ALMST",
    "PPMT",
    "AZST",
    "PGT",
    "GYT",
    "OMST",
    "KDT",
    "ARST",
    "PKST",
    "IDT",
    "FFMT",
    "LHST",
    "NRT",
    "SAKT",
    "AZT",
    "IRKT",
    "AMST",
    "NCST",
    "SST",
    "NOVST",
    "OMSST",
    "MUT",
    "IMT",
    "PMMT",
    "PST",
    "FNST",
    "MDDT",
    "JST",
    "VOLT",
    "MALST",
    "ADT",
    "MPT",
    "SAMT",
    "NUT",
    "AQTT",
    "CKHST",
    "MAGST",
    "EDDT",
    "BDST",
    "IST",
    "HKST",
    "CAT",
    "HOVT",
    "TAHT",
    "TASST",
    "WEST",
    "ANAST",
    "CHAST",
    "WGT",
    "AFT",
    "NFDT",
    "FJT",
    "CPT",
    "ORAT",
    "DDUT",
    "MSD+3",
    "CXT",
    "GMT",
    "TSAT",
    "AZOT",
    "NZMT",
    "AEDT",
    "ACWDT",
    "EST",
    "MWT",
    "SRT",
    "PETT",
    "ANT",
    "QYZST",
    "NCT",
    "TOT",
    "WIT",
    "KMT",
    "NZDT",
    "RET",
    "AWT",
    "VLAST",
    "EAT",
    "KUYST",
    "+09",
    "SAKST",
    "URUT",
    "SVET",
    "UYST",
    "GAMT",
    "VUT",
    "KOST",
    "CLT",
    "WMT",
    "+11",
    "DMT",
    "QYZT",
    "GDT",
    "MALT",
    "CHOST",
    "PETST",
    "WST",
    "BMT",
    "JAVT",
    "YDT",
    "--12",
    "AMT",
    "IRKST",
    "BOT",
    "EASST",
    "BDT",
    "JDT",
    "SJMT",
    "PYST",
    "AHDT",
    "VLAT",
    "ASHT",
    "GET",
    "IRST",
    "PMDT",
    "URAT",
    "DUST",
    "UZT",
    "TRST",
    "MAGT",
    "YEKT",
    "EMT",
    "ChST",
    "AKDT"
};
