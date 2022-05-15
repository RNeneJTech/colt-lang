const COMMENT = /((?:\/\/.*?<br>)|(?:\/\*(:?.|\n)*\*\/))/g;
const FLOAT = /([0-9]*.[0-9]+)/g;
const INT = /\b((?:[0-9]+)|(?:0x[0-9a-fA-F]+)|(?:0b[01]+)|(?:0o[0-7]+))\b/g;
const STRING = /(\".*?\")/g;
const FUNCTION = /\b([a-zA-Z][a-zA-Z0-9]+)(?=\()/g;
const NAMESPACE = /\b([a-zA-Z][a-zA-Z0-9]*)(?=\::)/g;
const KEYWORD = /\b(int|float|double|uint|bool|char|cstr|template|typename|typeof|var|ref|in|auto|nointerpret|pure|@compiler|const|@macro)\b/g;
const FLOW = /\b(import|false|true|is|as|fn|if|elif|else|while|for|break|continue|switch|return)\b/g;
const IDENTIFIER = /\b([a-zA-Z_][a-zA-Z0-9_]*)(?= *[=;,+)])/g;
const OPERATOR = /([=><!+,;/*%\-]*)/g;

/**
 * Highlights a 'colt' code block, and writes it in place. 
 * @param {string} str 
 */
function highlight_colt(str)
{
	str = str.replace(IDENTIFIER, "<span class='identifier'>$1</span>");
	str = str.replace(NAMESPACE, "<span class='namespace'>$1</span>");
	str = str.replace(FUNCTION, "<span class='function'>$1</span>");
	str = str.replace(COMMENT, "<span class=comment>$1</span>");
	str = str.replace(FLOAT, "<span class='constant'>$1</span>");
	str = str.replace(INT, "<span class='constant'>$1</span>");
	str = str.replace(STRING, "<span class='string'>$1</span>");
	str = str.replace(KEYWORD, "<span class='keyword'>$1</span>");
	str = str.replace(FLOW, "<span class='flow'>$1</span>");
	str = "<span class=operator>" + str + "</span>";
	str = str.replace(/\n/g, '<br>');
	str = str.replace(/\t/g, '   ');
	str = '<pre>' + str + '</pre>';	

	document.write(str);
}