V        REM  +------------------------------------------------+
X        REM  | HACK.BAS      (c) 19100   fr33 v4r14bl3z       |
XV       REM  |                                                |
XX       REM  | Brute-forces passwords on UM vIX.0 systems.    |
XXV      REM  | Compile with Qvickbasic VII.0 or later:        |
XXX      REM  |    /bin/qbasic hack.bas                        |
XXXV     REM  | Then run:                                      |
XL       REM  |   ./hack.exe username                          |
XLV      REM  |                                                |
L        REM  | This program is for educational purposes only! |
LV       REM  +------------------------------------------------+
LX       REM
LXV      IF ARGS() > I THEN GOTO LXXXV
LXX      PRINT "usage: ./hack.exe username"
LXXV     PRINT CHR(X)
LXXX     END
LXXXV    REM
XC       REM get username from command line
XCV      DIM username AS STRING
C        username = ARG(II)
CV       REM  common words used in passwords
CX       DIM pwdcount AS INTEGER
CXV      pwdcount = LIII
CXX      DIM words(pwdcount) AS STRING
CXXV     words(I) = "airplane"
CXXX     words(II) = "alphabet"
CXXXV    words(III) = "aviator"
CXL      words(IV) = "bidirectional"
CXLV     words(V) = "changeme"
CL       words(VI) = "creosote"
CLV      words(VII) = "cyclone"
CLX      words(VIII) = "december"
CLXV     words(IX) = "dolphin"
CLXX     words(X) = "elephant"
CLXXV    words(XI) = "ersatz"
CLXXX    words(XII) = "falderal"
CLXXXV   words(XIII) = "functional"
CXC      words(XIV) = "future"
CXCV     words(XV) = "guitar"
CC       words(XVI) = "gymnast"
CCV      words(XVII) = "hello"
CCX      words(XVIII) = "imbroglio"
CCXV     words(XIX) = "january"
CCXX     words(XX) = "joshua"
CCXXV    words(XXI) = "kernel"
CCXXX    words(XXII) = "kingfish"
CCXXXV   words(XXIII) = "(\b.bb)(\v.vv)"
CCXL     words(XXIV) = "millennium"
CCXLV    words(XXV) = "monday"
CCL      words(XXVI) = "nemesis"
CCLV     words(XXVII) = "oatmeal"
CCLX     words(XXVIII) = "october"
CCLXV    words(XXIX) = "paladin"
CCLXX    words(XXX) = "pass"
CCLXXV   words(XXXI) = "password"
CCLXXX   words(XXXII) = "penguin"
CCLXXXV  words(XXXIII) = "polynomial"
CCXC     words(XXXIV) = "popcorn"
CCXCV    words(XXXV) = "qwerty"
CCC      words(XXXVI) = "sailor"
CCCV     words(XXXVII) = "swordfish"
CCCX     words(XXXVIII) = "symmetry"
CCCXV    words(XXXIX) = "system"
CCCXX    words(XL) = "tattoo"
CCCXXV   words(XLI) = "thursday"
CCCXXX   words(XLII) = "tinman"
CCCXXXV  words(XLIII) = "topography"
CCCXL    words(XLIV) = "unicorn"
CCCXLV   words(XLV) = "vader"
CCCL     words(XLVI) = "vampire"
CCCLV    words(XLVII) = "viper"
CCCLX    words(XLVIII) = "warez"
CCCLXV   words(XLIX) = "xanadu"
CCCLXX   words(L) = "xyzzy"
CCCLXXV  words(LI) = "zephyr"
CCCLXXX  words(LII) = "zeppelin"
CCCLXXXV words(LIII) = "zxcvbnm"
CCCXC    REM try each password
CCCXCV   REM PRINT "attempting hack with " + pwdcount + " passwords " + CHR(X)
CD       DIM i AS INTEGER
CDV      i = I
CDX      IF CHECKPASS(username, words(i)) THEN GOTO CDXXX
CDXV     i = i + I
CDXX     IF i > pwdcount THEN GOTO CDXLV
CDXXV    GOTO CDX
CDXXX    REM PRINT "found match!! for user " + username + CHR(X)
CDXXXV   PRINT username + " : " + words(i) + CHR(X)
CDXL     END
CDXLV    REM PRINT "no simple matches for user " + username + CHR(X)

CDL      REM
CDLV     REM  the above code will probably crack passwords for many
CDLX     REM  users so I always try it first. when it fails, I try the
CDLXV    REM  more expensive method below.
CDLXX    REM
CDLXXV   REM  passwords often take the form
CDLXXX   REM    dictwordDD
CDLXXXV  REM  where DD is a two-digit decimal number. try these next:

CDXCV    DIM j AS INTEGER
D        DIM k AS INTEGER
DV       DIM d(X) AS STRING
DVI      DIM pass AS STRING
DX       d(I) = "1"
DXV      d(II) = "2"
DXX      d(III) = "3"
DXXV     d(IV) = "4"
DXXX     d(V) = "5"
DXXXV    d(VI) = "6"
DXL      d(VII) = "7"
DXLV     d(VIII) = "8"
DL       d(IX) = "9"
DLV      d(X) = "0"

DLVI     i = I
DLX      j = I
DLXV     k = I
DLXX     pass = words(i) + d(j) + d(k)
DLXXV    IF CHECKPASS(username, pass) THEN GOTO DCXX
DLXXX    k = k + I
DLXXXV   IF k <= X THEN GOTO DLXX
DXC      j = j + I
DXCV     IF j <= X THEN GOTO DLXV
DC       i = i + I
DCV      IF i <= pwdcount THEN GOTO DLX

DCX      PRINT "no matches for user " + username + CHR(X)
DCXV     END

DCXX     PRINT username + " : " + pass + CHR(X)
DCXXV    END
