echo "Formatting: " > /dev/tty
find Sources/ \
 \( -ipath "*.cpp" -o -ipath "*.h" \) \
 "!" -path "Sources/flatbuffers/*" \
 "!" -iname "*enet*" \
 "!" -name "*Generated.h" \
 "!" -iname "*simdjson*" \
 | tee /dev/tty | xargs clang-format -i
