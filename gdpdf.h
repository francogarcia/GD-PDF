/* gdpdf.h */

#ifndef GD_PDF_H
#define GD_PDF_H

#include "map.h"
#include "reference.h"
#include "ustring.h"

// HARU PDF library.
#include "libharu/include/hpdf.h"

class PDF : public Reference {
	OBJ_TYPE(PDF, Reference);

	HPDF_Doc m_PDF;

	HPDF_Page m_CurrentPage;
	HPDF_REAL m_PageHeight;
	HPDF_REAL m_PageWidth;

	HPDF_REAL m_CurrentPageWidth;
	HPDF_REAL m_CurrentPageHeight;

	// Box dimensions which will contain the written text.
	HPDF_REAL m_CurrentLeft;
	HPDF_REAL m_CurrentTop;
	HPDF_REAL m_CurrentRight;
	HPDF_REAL m_CurrentBottom;

	// Dimensions for the text area.
	HPDF_REAL m_Header;
	HPDF_REAL m_Footer;
	HPDF_REAL m_LeftMargin;
	HPDF_REAL m_RightMargin;

	// Font sizes and layout.
	// Size of a paragraph break.
	HPDF_REAL m_TextSize;
	HPDF_REAL m_ParagraphSpacing;

	// Fonts.
	HPDF_Font m_FontSansSerif;
	HPDF_Font m_FontSerif;

	// Error code.
	HPDF_STATUS m_Status;

protected:
	static void _bind_methods();

public:
	PDF();

	// TODO: Application specific. This should go to GDScript.
	bool add_section(const String& title, const String& subtitle);

	// Create a PDF file using UTF-8 encoding.
	// To create a page to write on, call add_page().
	bool create_document(const String& sansSerifFontPath,
						 const String& serifFontPath);

	// Add a new page to the document.
	bool add_new_page();

	bool add_title(const String& content);

	bool add_header(const String& content);

	bool add_body_text(const String& content);

	bool save_document(const String& filePath);

	bool finish_document();

private:
	// Updates the values of the page attribute, to place the next text in the
	// next line.
	void set_new_page_attributes();

	// Add text formatted as title to the PDF.
	void create_title(HPDF_Page& page,
					  const HPDF_Font& font,
					  HPDF_STATUS& status,
					  HPDF_REAL& currentPageHeight,
					  HPDF_REAL& currentPageWidth,
					  const HPDF_REAL left,
					  const HPDF_REAL top,
					  const HPDF_REAL right,
					  const HPDF_REAL bottom,
					  const String& text) const;

	// Add text formatted as a header to the PDF.
	void create_header(HPDF_Page& page,
					   const HPDF_Font& font,
					   HPDF_STATUS& status,
					   HPDF_REAL& currentPageHeight,
					   HPDF_REAL& currentPageWidth,
					   const HPDF_REAL left,
					   const HPDF_REAL top,
					   const HPDF_REAL right,
					   const HPDF_REAL bottom,
					   const String& text) const;

	// Add text formatted as body text to the PDF.
	void create_body(HPDF_Page& page,
					 const HPDF_Font& font,
					 HPDF_STATUS& status,
					 HPDF_REAL& currentPageHeight,
					 HPDF_REAL& currentPageWidth,
					 const HPDF_REAL left,
					 const HPDF_REAL top,
					 const HPDF_REAL right,
					 const HPDF_REAL bottom,
					 const String& text) const;

	// Add text to the PDF.
	void create_text(HPDF_Page& page,
					 const HPDF_Font& font,
					 const HPDF_REAL fontSize,
					 const HPDF_REAL fontSpacingFactor,
					 HPDF_STATUS& status,
					 HPDF_REAL& currentPageHeight,
					 HPDF_REAL& currentPageWidth,
					 const HPDF_REAL left,
					 const HPDF_REAL top,
					 const HPDF_REAL right,
					 const HPDF_REAL bottom,
					 const HPDF_TextAlignment align,
					 const String& text) const;

	// Fix page attributes after writing text.
	// Set next line below of the end of the current one.
	void set_up_page(HPDF_REAL& currentPageHeight,
					 HPDF_REAL& currentPageWidth,
					 HPDF_REAL& left,
					 HPDF_REAL& top,
					 HPDF_REAL& right,
					 HPDF_REAL& bottom,
					 HPDF_REAL paragraphSpacing,
					 const HPDF_REAL pageWidth,
					 const HPDF_REAL header,
					 const HPDF_REAL footer,
					 const HPDF_REAL leftMargin,
					 const HPDF_REAL rightMargin) const;

	// Fix page attributes after writing text.
	// Set next line in front of the end of the current one.
	// (Warning: this only work for short strings).
	void set_up_page_in_front(HPDF_REAL& currentPageHeight,
							  HPDF_REAL& currentPageWidth,
							  HPDF_REAL& left,
							  HPDF_REAL& top,
							  HPDF_REAL& right,
							  HPDF_REAL& bottom,
							  HPDF_REAL paragraphSpacing,
							  const HPDF_REAL pageWidth,
							  const HPDF_REAL header,
							  const HPDF_REAL footer,
							  const HPDF_REAL leftMargin,
							  const HPDF_REAL rightMargin) const;

	// Add text formatted as a section to the PDF.
	void create_section(HPDF_Page& page,
						const HPDF_Font& headerFont,
						const HPDF_Font& bodyFont,
						const String& headerText,
						const String& bodyText,
						HPDF_STATUS& status,
						HPDF_REAL& currentPageHeight,
						HPDF_REAL& currentPageWidth,
						HPDF_REAL& left,
						HPDF_REAL& top,
						HPDF_REAL& right,
						HPDF_REAL& bottom,
						HPDF_REAL paragraphSpacing,
						const HPDF_REAL pageWidth,
						const HPDF_REAL header,
						const HPDF_REAL footer,
						const HPDF_REAL leftMargin,
						const HPDF_REAL rightMargin) const;
};

#endif

// To change spaces to tabs: M-x tabify
// Local Variables:
// mode: c++
// tab-width: 4
// c-basic-offset: 4
// indent-tabs-mode: t
// End:
