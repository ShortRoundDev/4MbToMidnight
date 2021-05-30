##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Release
ProjectName            :=4MbToMidnight
ConfigurationName      :=Release
WorkspacePath          :=/home/collin/Code/4MbToMidnight
ProjectPath            :=/home/collin/Code/4MbToMidnight
IntermediateDirectory  :=./Release
OutDir                 := $(IntermediateDirectory)
CurrentFileName        :=
CurrentFilePath        :=
CurrentFileFullPath    :=
User                   :=Collin Oswalt
Date                   :=30/05/21
CodeLitePath           :=/home/collin/.codelite
LinkerName             :=/usr/bin/g++
SharedObjectLinkerName :=/usr/bin/g++ -shared -fPIC
ObjectSuffix           :=.o
DependSuffix           :=.o.d
PreprocessSuffix       :=.i
DebugSwitch            :=-g 
IncludeSwitch          :=-I
LibrarySwitch          :=-l
OutputSwitch           :=-o 
LibraryPathSwitch      :=-L
PreprocessorSwitch     :=-D
SourceSwitch           :=-c 
OutputFile             :=$(IntermediateDirectory)/$(ProjectName)
Preprocessors          :=$(PreprocessorSwitch)NDEBUG 
ObjectSwitch           :=-o 
ArchiveOutputSwitch    := 
PreprocessOnlySwitch   :=-E
ObjectsFileList        :="4MbToMidnight.txt"
PCHCompileFlags        :=
MakeDirCommand         :=mkdir -p
LinkOptions            :=  
IncludePath            :=  $(IncludeSwitch). $(IncludeSwitch). $(IncludeSwitch)include 
IncludePCH             := 
RcIncludePath          := 
Libs                   := $(LibrarySwitch)glfw $(LibrarySwitch)GL $(LibrarySwitch)dl $(LibrarySwitch)openal $(LibrarySwitch)vorbisfile 
ArLibs                 :=  "glfw" "GL" "dl" "openal" "vorbisfile" 
LibPath                := $(LibraryPathSwitch). 

##
## Common variables
## AR, CXX, CC, AS, CXXFLAGS and CFLAGS can be overriden using an environment variables
##
AR       := /usr/bin/ar rcu
CXX      := /usr/bin/g++
CC       := /usr/bin/gcc
CXXFLAGS :=  -O2 -Wall $(Preprocessors)
CFLAGS   :=  -O2 -Wall $(Preprocessors)
ASFLAGS  := 
AS       := /usr/bin/as


##
## User defined environment variables
##
CodeLiteDir:=/usr/share/codelite
Objects0=$(IntermediateDirectory)/SoundManager.cpp$(ObjectSuffix) $(IntermediateDirectory)/GraphicsManager.cpp$(ObjectSuffix) $(IntermediateDirectory)/BlueKey.cpp$(ObjectSuffix) $(IntermediateDirectory)/glad.c$(ObjectSuffix) $(IntermediateDirectory)/Camera.cpp$(ObjectSuffix) $(IntermediateDirectory)/Entity.cpp$(ObjectSuffix) $(IntermediateDirectory)/Shader.cpp$(ObjectSuffix) $(IntermediateDirectory)/Player.cpp$(ObjectSuffix) $(IntermediateDirectory)/Wall.cpp$(ObjectSuffix) $(IntermediateDirectory)/Level.cpp$(ObjectSuffix) \
	$(IntermediateDirectory)/GameManager.cpp$(ObjectSuffix) $(IntermediateDirectory)/main.cpp$(ObjectSuffix) 



Objects=$(Objects0) 

##
## Main Build Targets 
##
.PHONY: all clean PreBuild PrePreBuild PostBuild MakeIntermediateDirs
all: $(OutputFile)

$(OutputFile): $(IntermediateDirectory)/.d $(Objects) 
	@$(MakeDirCommand) $(@D)
	@echo "" > $(IntermediateDirectory)/.d
	@echo $(Objects0)  > $(ObjectsFileList)
	$(LinkerName) $(OutputSwitch)$(OutputFile) @$(ObjectsFileList) $(LibPath) $(Libs) $(LinkOptions)

PostBuild:
	@echo Executing Post Build commands ...
	cp *Shader.glsl Release/
	cp -r Resources Release/
	@echo Done

MakeIntermediateDirs:
	@test -d ./Release || $(MakeDirCommand) ./Release


$(IntermediateDirectory)/.d:
	@test -d ./Release || $(MakeDirCommand) ./Release

PreBuild:


##
## Objects
##
$(IntermediateDirectory)/SoundManager.cpp$(ObjectSuffix): SoundManager.cpp $(IntermediateDirectory)/SoundManager.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/collin/Code/4MbToMidnight/SoundManager.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/SoundManager.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/SoundManager.cpp$(DependSuffix): SoundManager.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/SoundManager.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/SoundManager.cpp$(DependSuffix) -MM SoundManager.cpp

$(IntermediateDirectory)/SoundManager.cpp$(PreprocessSuffix): SoundManager.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/SoundManager.cpp$(PreprocessSuffix) SoundManager.cpp

$(IntermediateDirectory)/GraphicsManager.cpp$(ObjectSuffix): GraphicsManager.cpp $(IntermediateDirectory)/GraphicsManager.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/collin/Code/4MbToMidnight/GraphicsManager.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/GraphicsManager.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/GraphicsManager.cpp$(DependSuffix): GraphicsManager.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/GraphicsManager.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/GraphicsManager.cpp$(DependSuffix) -MM GraphicsManager.cpp

$(IntermediateDirectory)/GraphicsManager.cpp$(PreprocessSuffix): GraphicsManager.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/GraphicsManager.cpp$(PreprocessSuffix) GraphicsManager.cpp

$(IntermediateDirectory)/BlueKey.cpp$(ObjectSuffix): BlueKey.cpp $(IntermediateDirectory)/BlueKey.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/collin/Code/4MbToMidnight/BlueKey.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/BlueKey.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/BlueKey.cpp$(DependSuffix): BlueKey.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/BlueKey.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/BlueKey.cpp$(DependSuffix) -MM BlueKey.cpp

$(IntermediateDirectory)/BlueKey.cpp$(PreprocessSuffix): BlueKey.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/BlueKey.cpp$(PreprocessSuffix) BlueKey.cpp

$(IntermediateDirectory)/glad.c$(ObjectSuffix): glad.c $(IntermediateDirectory)/glad.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/home/collin/Code/4MbToMidnight/glad.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/glad.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/glad.c$(DependSuffix): glad.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/glad.c$(ObjectSuffix) -MF$(IntermediateDirectory)/glad.c$(DependSuffix) -MM glad.c

$(IntermediateDirectory)/glad.c$(PreprocessSuffix): glad.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/glad.c$(PreprocessSuffix) glad.c

$(IntermediateDirectory)/Camera.cpp$(ObjectSuffix): Camera.cpp $(IntermediateDirectory)/Camera.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/collin/Code/4MbToMidnight/Camera.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Camera.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Camera.cpp$(DependSuffix): Camera.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Camera.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Camera.cpp$(DependSuffix) -MM Camera.cpp

$(IntermediateDirectory)/Camera.cpp$(PreprocessSuffix): Camera.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Camera.cpp$(PreprocessSuffix) Camera.cpp

$(IntermediateDirectory)/Entity.cpp$(ObjectSuffix): Entity.cpp $(IntermediateDirectory)/Entity.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/collin/Code/4MbToMidnight/Entity.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Entity.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Entity.cpp$(DependSuffix): Entity.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Entity.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Entity.cpp$(DependSuffix) -MM Entity.cpp

$(IntermediateDirectory)/Entity.cpp$(PreprocessSuffix): Entity.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Entity.cpp$(PreprocessSuffix) Entity.cpp

$(IntermediateDirectory)/Shader.cpp$(ObjectSuffix): Shader.cpp $(IntermediateDirectory)/Shader.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/collin/Code/4MbToMidnight/Shader.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Shader.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Shader.cpp$(DependSuffix): Shader.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Shader.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Shader.cpp$(DependSuffix) -MM Shader.cpp

$(IntermediateDirectory)/Shader.cpp$(PreprocessSuffix): Shader.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Shader.cpp$(PreprocessSuffix) Shader.cpp

$(IntermediateDirectory)/Player.cpp$(ObjectSuffix): Player.cpp $(IntermediateDirectory)/Player.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/collin/Code/4MbToMidnight/Player.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Player.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Player.cpp$(DependSuffix): Player.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Player.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Player.cpp$(DependSuffix) -MM Player.cpp

$(IntermediateDirectory)/Player.cpp$(PreprocessSuffix): Player.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Player.cpp$(PreprocessSuffix) Player.cpp

$(IntermediateDirectory)/Wall.cpp$(ObjectSuffix): Wall.cpp $(IntermediateDirectory)/Wall.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/collin/Code/4MbToMidnight/Wall.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Wall.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Wall.cpp$(DependSuffix): Wall.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Wall.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Wall.cpp$(DependSuffix) -MM Wall.cpp

$(IntermediateDirectory)/Wall.cpp$(PreprocessSuffix): Wall.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Wall.cpp$(PreprocessSuffix) Wall.cpp

$(IntermediateDirectory)/Level.cpp$(ObjectSuffix): Level.cpp $(IntermediateDirectory)/Level.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/collin/Code/4MbToMidnight/Level.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Level.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Level.cpp$(DependSuffix): Level.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Level.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Level.cpp$(DependSuffix) -MM Level.cpp

$(IntermediateDirectory)/Level.cpp$(PreprocessSuffix): Level.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Level.cpp$(PreprocessSuffix) Level.cpp

$(IntermediateDirectory)/GameManager.cpp$(ObjectSuffix): GameManager.cpp $(IntermediateDirectory)/GameManager.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/collin/Code/4MbToMidnight/GameManager.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/GameManager.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/GameManager.cpp$(DependSuffix): GameManager.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/GameManager.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/GameManager.cpp$(DependSuffix) -MM GameManager.cpp

$(IntermediateDirectory)/GameManager.cpp$(PreprocessSuffix): GameManager.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/GameManager.cpp$(PreprocessSuffix) GameManager.cpp

$(IntermediateDirectory)/main.cpp$(ObjectSuffix): main.cpp $(IntermediateDirectory)/main.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/collin/Code/4MbToMidnight/main.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/main.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/main.cpp$(DependSuffix): main.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/main.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/main.cpp$(DependSuffix) -MM main.cpp

$(IntermediateDirectory)/main.cpp$(PreprocessSuffix): main.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/main.cpp$(PreprocessSuffix) main.cpp


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) -r ./Release/


