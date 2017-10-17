git archive -o hot-fix-20170816.zip HEAD $( git diff e8ee9351b761e6362d02f6c58d792d90eb6fde6a dfec69778ad81ca1b941895b1fbf4915f3d3f977 --name-only)


基于版本
266280ef105ca919e5b82a0ead4a7f3decb9f066

1.请将buildtools下面的ninja.exe ,7z.exe,7z.dll拷到python2.7.6的安装目录下，并配置好python.
2.请从此处 http://pan.baidu.com/s/1dFF7kf3 下载vs2015.zip，并解压到C:\vs2015_files目录下．
3.调用gen.bat生成工程
4.打开build.bat编绎(如果有安装visual studio的话，也可以直接打开out/Release目录下面的All.sln进行编绎)

如果不想下载百度盘的东西，想直接使用系统安装的visual studio编绎的话，可以修改env.bat脚本．
Set DEPOT_TOOLS_WIN_TOOLCHAIN=0 即可

目前SOUI内核支持用clang编绎，其它模块暂不支持
如果想黉试clang编绎SOUI内核，请修改build/config/BUILDCONFIG.gn中141行，
将   (current_os == "win" && host_os != "win") 
改为 (current_os == "win") 
然后重复以上1,2,3,4即可

