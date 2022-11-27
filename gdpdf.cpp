/* gdpdf.cpp */

#include "gdpdf.h"

//jmp_buf env;

#ifdef HPDF_DLL
void  __stdcall
#else
void
#endif
HARU_error_handler(HPDF_STATUS error_no, HPDF_STATUS detail_no, void *user_data)
{
	//printf("ERROR: error_no=%04X, detail_no=%u\n", (HPDF_UINT) error_no, (HPDF_UINT) detail_no);
	//longjmp(env, 1);
}

void PDF::_bind_methods() {

	ClassDB::bind_method(D_METHOD("add_section"), &PDF::add_section);
	ClassDB::bind_method(D_METHOD("create_document"), &PDF::create_document);
	ClassDB::bind_method(D_METHOD("add_new_page"), &PDF::add_new_page);
	ClassDB::bind_method(D_METHOD("add_title"), &PDF::add_title);
	ClassDB::bind_method(D_METHOD("add_header"), &PDF::add_header);
	ClassDB::bind_method(D_METHOD("add_body_text"), &PDF::add_body_text);
	ClassDB::bind_method(D_METHOD("save_document"), &PDF::save_document);
	ClassDB::bind_method(D_METHOD("finish_document"), &PDF::finish_document);
}

PDF::PDF() {

	m_PDF = NULL;

	m_Header = 20.0f;
	m_Footer = 20.0f;
	m_LeftMargin = 20.0f;
	m_RightMargin = 20.0f;

	m_CurrentPage = NULL;

	m_CurrentPageWidth = 0.0f;
	m_CurrentPageHeight = 0.0f;

	m_TextSize = 12.0f;
	m_ParagraphSpacing = m_TextSize;
}

bool PDF::create_document(const String& sansSerifFontPath,
						  const String& serifFontPath) {

	m_PDF = HPDF_New(HARU_error_handler, NULL);
	if (!m_PDF) {
		return false;
	}

	// if (setjmp(g_HaruEnv)) {
	//		HPDF_Free(pdf);
	//		return false;
	// }

	// Set compression mode.
	HPDF_SetCompressionMode(m_PDF, HPDF_COMP_ALL);

	// Set page mode to use outlines.
	HPDF_SetPageMode(m_PDF, HPDF_PAGE_MODE_USE_OUTLINE);

	// Set document password.
	//HPDF_SetPassword (m_PDF, "owner", "user");

	// Set character encoding to UTF-8.
	HPDF_UseUTFEncodings(m_PDF);

	// Load a font with UTF-8 encoding.
	String fontName = HPDF_LoadTTFontFromFile(m_PDF, sansSerifFontPath.utf8().get_data(), HPDF_TRUE);
	m_FontSansSerif = HPDF_GetFont(m_PDF, fontName.utf8().get_data(), "UTF-8");
	fontName = HPDF_LoadTTFontFromFile(m_PDF, serifFontPath.utf8().get_data(), HPDF_TRUE);
	m_FontSerif = HPDF_GetFont(m_PDF, fontName.utf8().get_data(), "UTF-8");

	return true;
}

bool PDF::add_new_page() {

	m_CurrentPage = HPDF_AddPage(m_PDF);

	// Set the attributes of the page.
	HPDF_Page_SetSize(m_CurrentPage, HPDF_PAGE_SIZE_A4, HPDF_PAGE_PORTRAIT);

	m_PageHeight = HPDF_Page_GetHeight(m_CurrentPage);
	m_PageWidth = HPDF_Page_GetWidth(m_CurrentPage);

	m_CurrentPageWidth = 0.0f;
	m_CurrentPageHeight = 0.0f;

	// Box dimensions which will contain the written text.
	m_CurrentLeft = m_LeftMargin;
	m_CurrentTop = m_PageHeight - m_Header - m_CurrentPageHeight;
	m_CurrentRight = m_PageWidth - m_RightMargin;
	m_CurrentBottom = m_Footer;

	return true;
}

bool PDF::add_title(const String& content) {

	create_title(m_CurrentPage,
				 m_FontSerif,
				 m_Status,
				 m_CurrentPageHeight,
				 m_CurrentPageWidth,
				 m_CurrentLeft,
				 m_CurrentTop,
				 m_CurrentRight,
				 m_CurrentBottom,
				 content);
	set_new_page_attributes();

	return true;
}

bool PDF::add_header(const String& content) {

	create_header(m_CurrentPage,
				  m_FontSerif,
				  m_Status,
				  m_CurrentPageHeight,
				  m_CurrentPageWidth,
				  m_CurrentLeft,
				  m_CurrentTop,
				  m_CurrentRight,
				  m_CurrentBottom,
				  content);
	set_new_page_attributes();

	return true;
}

bool PDF::add_body_text(const String& content) {

	create_body(m_CurrentPage,
				m_FontSerif,
				m_Status,
				m_CurrentPageHeight,
				m_CurrentPageWidth,
				m_CurrentLeft,
				m_CurrentTop,
				m_CurrentRight,
				m_CurrentBottom,
				content);
	set_new_page_attributes();

	return true;
}

bool PDF::save_document(const String& filePath) {

	m_Status = HPDF_SaveToFile(m_PDF, filePath.utf8().get_data());
	//assert(m_Status == HPDF_OK);

	return true;
}

bool PDF::finish_document() {

	// Clean-up.
	HPDF_Free(m_PDF);

	return true;
}

void PDF::set_new_page_attributes()
{

	set_up_page(m_CurrentPageHeight,
				m_CurrentPageWidth,
				m_CurrentLeft,
				m_CurrentTop,
				m_CurrentRight,
				m_CurrentBottom,
				m_ParagraphSpacing,
				m_CurrentPageWidth,
				m_Header,
				m_Footer,
				m_LeftMargin,
				m_RightMargin);
}

void PDF::create_title(HPDF_Page& page,
					   const HPDF_Font& font,
					   HPDF_STATUS& status,
					   HPDF_REAL& currentPageHeight,
					   HPDF_REAL& currentPageWidth,
					   const HPDF_REAL left,
					   const HPDF_REAL top,
					   const HPDF_REAL right,
					   const HPDF_REAL bottom,
					   const String& text) const {

	// Size of the text.
	const HPDF_REAL titleSize = 24.0f;
	// Text alignment.
	HPDF_TextAlignment align = HPDF_TALIGN_CENTER;
	// Multiplier for the line spacing of wrapped text.
	HPDF_REAL lineSpacingFactor = 1.5f;
	// Size of a paragraph break.
	//HPDF_REAL paragraphSpacing = 12.0f; //textSize;

	create_text(page, font, titleSize, lineSpacingFactor, status,
				currentPageHeight, currentPageWidth,
				left, top, right, bottom,
				align, text);
}

void PDF::create_header(HPDF_Page& page,
						const HPDF_Font& font,
						HPDF_STATUS& status,
						HPDF_REAL& currentPageHeight,
						HPDF_REAL& currentPageWidth,
						const HPDF_REAL left,
						const HPDF_REAL top,
						const HPDF_REAL right,
						const HPDF_REAL bottom,
						const String& text) const {

	// Size of the text.
	const HPDF_REAL headerSize = 16.0f;
	// Text alignment.
	HPDF_TextAlignment align = HPDF_TALIGN_JUSTIFY;
	// Multiplier for the line spacing of wrapped text.
	HPDF_REAL lineSpacingFactor = 1.5f;
	// Size of a paragraph break.
	//HPDF_REAL paragraphSpacing = 12.0f; //textSize;

	create_text(page, font, headerSize, lineSpacingFactor, status,
				currentPageHeight, currentPageWidth,
				left, top, right, bottom,
				align, text);
}

void PDF::create_body(HPDF_Page& page,
					  const HPDF_Font& font,
					  HPDF_STATUS& status,
					  HPDF_REAL& currentPageHeight,
					  HPDF_REAL& currentPageWidth,
					  const HPDF_REAL left,
					  const HPDF_REAL top,
					  const HPDF_REAL right,
					  const HPDF_REAL bottom,
					  const String& text) const {

	// Size of the text.
	const HPDF_REAL textSize = 12.0f;
	// Text alignment.
	HPDF_TextAlignment align = HPDF_TALIGN_LEFT;
	// Multiplier for the line spacing of wrapped text.
	HPDF_REAL lineSpacingFactor = 1.5f;
	// Size of a paragraph break.
	//HPDF_REAL paragraphSpacing = textSize;

	create_text(page, font, textSize, lineSpacingFactor, status,
				currentPageHeight, currentPageWidth,
				left, top, right, bottom,
				align, text);
}

void PDF::create_text(HPDF_Page& page,
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
					  const String& text) const {

	HPDF_Page_SetFontAndSize(page, font, fontSize);
	HPDF_Page_SetTextLeading(page, fontSize * fontSpacingFactor);
	HPDF_Page_BeginText(page);
	// Write text in the are of a rectangle. Wrap text, if needed.
	status = HPDF_Page_TextRect(page,
								left, top, right, bottom,
								text.utf8().get_data(),
								align,
								NULL);
	//assert(status == HPDF_OK);

	// Resets page width if the current height changes.
	// This avoids making the next line narrower than the previous one.
	if (currentPageHeight != HPDF_Page_GetCurrentTextPos(page).y) {
		currentPageWidth = m_PageWidth - m_RightMargin;
	} else {
		currentPageWidth = HPDF_Page_GetCurrentTextPos(page).x;
	}
	currentPageHeight = HPDF_Page_GetCurrentTextPos(page).y;// + HPDF_Page_GetCurrentFontSize(page);

	HPDF_Page_EndText(page);
}

void PDF::set_up_page(HPDF_REAL& currentPageHeight,
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
					  const HPDF_REAL rightMargin) const {

	currentPageHeight -= paragraphSpacing;
	left = leftMargin;
	top = currentPageHeight;
	right = pageWidth - rightMargin;
	bottom = footer;
}

void PDF::set_up_page_in_front(HPDF_REAL& currentPageHeight,
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
							   const HPDF_REAL rightMargin) const {
	// Size of the text.
	const HPDF_REAL textSize = 16.0f;

	//currentPageHeight -= paragraphSpacing;
	left = currentPageWidth + textSize / 2.0f;
	top = currentPageHeight + textSize;
	right = pageWidth - rightMargin;
	bottom = footer;
}

void PDF::create_section(HPDF_Page& page,
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
						 const HPDF_REAL rightMargin) const {

	// LibHARU stops writing to the PDF if it receives an empty string.

	// Header.
	create_header(page, headerFont, status,
				  currentPageHeight, currentPageWidth,
				  left, top, right, bottom, headerText);

	// Body.
	if (bodyText != "") {
		set_up_page_in_front(currentPageHeight, currentPageWidth,
							 left, top, right, bottom,
							 paragraphSpacing, pageWidth,
							 header, footer, leftMargin, rightMargin);
		create_body(page, bodyFont, status,
					currentPageHeight, currentPageWidth,
					left, top, right, bottom, bodyText);
	}

	set_up_page(currentPageHeight, currentPageWidth,
				left, top, right, bottom,
				paragraphSpacing, pageWidth,
				header, footer, leftMargin, rightMargin);
}

bool PDF::add_section(const String& title, const String& subtitle)
{

	create_section(m_CurrentPage,
				   m_FontSerif,
				   m_FontSansSerif,
				   title,
				   subtitle,
				   m_Status,
				   m_CurrentPageHeight,
				   m_CurrentPageWidth,
				   m_CurrentLeft,
				   m_CurrentTop,
				   m_CurrentRight,
				   m_CurrentBottom,
				   m_ParagraphSpacing,
				   m_PageWidth,
				   m_Header,
				   m_Footer,
				   m_LeftMargin,
				   m_RightMargin);
	//set_new_page_attributes();

	return true;
}

// To change spaces to tabs: M-x tabify
// Local Variables:
// mode: c++
// tab-width: 4
// c-basic-offset: 4
// indent-tabs-mode: t
// End:
