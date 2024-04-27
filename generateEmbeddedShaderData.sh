if [ -f vis/embeddedShaderData.h ]; then
    rm vis/embeddedShaderData.h
fi

echo "const char * embeddedShaderData = R\"0B3R0N(\n" > vis/embeddedShaderData.h

cat vis/shaderData.h >> vis/embeddedShaderData.h

echo "\n)0B3R0N\";" >> vis/embeddedShaderData.h
