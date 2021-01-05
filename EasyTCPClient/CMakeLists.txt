set(ProjectName "EasyTCPClient")
project(${ProjectName})
set(SHARE_LIBS "")				# 全局依赖库的路径
set(SOURCE_FILES "")			# 全局源文件的路径
#添加源文件
set(fileList "")
# 递归遍历目录，生成多级目录工程
function (getfiles dir mark)
	set(filesInFolder "")
	file(GLOB files RELATIVE ${dir} ${dir}/*)
	foreach (file ${files})
		set(fullfilepath ${dir}/${file})
		if (IS_DIRECTORY ${fullfilepath})	
			getfiles(${fullfilepath} \\${mark}\\${file})
			include_directories(${fullfilepath})
		else()
			list(APPEND filesInFolder ${fullfilepath})			
		endif()
	endforeach()
    # 代码归组
	source_group(${mark} FILES ${filesInFolder})
	set(fileList ${fileList} ${filesInFolder} PARENT_SCOPE)
endfunction()

getfiles("${PROJECT_SOURCE_DIR}/src" "src")
list(APPEND SOURCE_FILES ${fileList})

# 生成可执行文件
add_executable(${ProjectName} ${SOURCE_FILES})