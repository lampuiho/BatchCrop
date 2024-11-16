#define DEFINE_INCREMENT( ENUM ) \
    friend ENUM& operator++(ENUM& enumVal); \
    friend ENUM operator++(ENUM& enumVal, int postIncrement);

#define PRE_AND_POST_INCREMENT( ENUM )                        \
inline ENUM& operator++( ENUM& enumVal ) {                    \
  return (ENUM&)(++(int&)enumVal);                            \
}                                                             \
inline ENUM operator++( ENUM& enumVal, int postIncrement ) {  \
  ENUM oldValue = enumVal;                                    \
  ++enumVal;                                                  \
  return oldValue;                                            \
}
