#
# Generates the document using pdflatex.
#

if(ENABLE_DESIGN_DOCUMENT)
	include(UseLATEX)
	# Abort compilation on errors.
	# Failing to do so might lead to infinite loops and hard to diagnose
	# problems.
	set(LATEX_COMPILER_FLAGS "")
	set(PDFLATEX_COMPILER_FLAGS "")

	ADD_LATEX_DOCUMENT(
		design.tex
	INPUTS
		tex/preface.tex
		tex/design.tex
		tex/coding_style.tex
		tex/libraries.tex
		tex/modules.tex
		tex/communication.tex
		tex/protocol.tex
		tex/types.tex
		tex/client.tex
	DEFAULT_PDF
	MANGLE_TARGET_NAMES
	)
endif(ENABLE_DESIGN_DOCUMENT)

