
LIBRARYPATHS = "_libraries"
ASMFILE = "out"
MACHINEFILE = "out.bbx"
PROFILECOMPILER = #--profile
VIEWEXECUTION = #--view-short
PRINTASSEMBLERSTATS = #--print-stats
EMITINTERMEDIATE = --emit-offsets --emit-tac --emit-pil
EMITINDICES = #--emit-indices
MAXINSTRUCTIONS = 1000000
INCLUDECOMMENTS = #--include-comments

all:
	@clear
	pmax build --target-name $(ASMFILE) --lib-paths $(LIBRARYPATHS) $(PROFILECOMPILER) $(EMITINTERMEDIATE) $(INCLUDECOMMENTS)
	bbasm assemble _targets/$(ASMFILE).bba _targets/$(MACHINEFILE) $(EMITINDICES) $(PRINTASSEMBLERSTATS)
	bbvm run _targets/$(MACHINEFILE) $(VIEWEXECUTION) --max-instructions $(MAXINSTRUCTIONS)
