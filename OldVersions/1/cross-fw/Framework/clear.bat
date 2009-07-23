del /Q Bin\Debug\*
del /Q Bin\Release\*

del /Q Libs\Debug\*
del /Q Libs\Release\*

del /Q Objects\Debug\Common\*
del /Q Objects\Release\Common\*

del /Q Objects\Debug\System\*
del /Q Objects\Release\System\*

del /Q Objects\Debug\System\Platform\*
del /Q Objects\Release\System\Platform\*

del /Q Objects\Debug\FWLoader\*
del /Q Objects\Release\FWLoader\*

del /Q Objects\Debug\Registry\*
del /Q Objects\Release\Registry\*

del /Q Objects\Debug\TinyXml\*
del /Q Objects\Release\TinyXml\*

del /Q Objects\Debug\Tools\*
del /Q Objects\Release\Tools\*

del /Q Objects\Debug\ClassFactory\*
del /Q Objects\Release\ClassFactory\*

del /Q Objects\Debug\CmpRegClient\*
del /Q Objects\Release\CmpRegClient\*

del /Q Objects\Debug\ServiceManager\*
del /Q Objects\Release\ServiceManager\*

del /Q Objects\Debug\TestService\*
del /Q Objects\Release\TestService\*

del /Q Objects\Debug\TestService1\*
del /Q Objects\Release\TestService1\*

del /Q Objects\Debug\TestService2\*
del /Q Objects\Release\TestService2\*

copy Bin\Config.xml Bin\Debug\Config.xml
copy Bin\Config.xml Bin\Release\Config.xml

copy Bin\Registry.xml Bin\Debug\Registry.xml
copy Bin\Registry.xml Bin\Release\Registry.xml
