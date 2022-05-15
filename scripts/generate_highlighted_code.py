# Generates an HTML file with highlighted code, using resources/highlight_code/highlight.js
# The code examples are taking from the resources directory, from every file ending with '.ct'

import os

print("-- Current working directory: ", os.getcwd())
if (os.getcwd().endswith("scripts")) or os.getcwd().endswith("scripts/"):
	os.chdir("..")
	print("-- Changed working directory to the parent directory")

files = []
try:
	files = os.listdir("resources")
	print("-- Listing files in 'resources' directory")
except:
	print("-- FATAL: Directory 'resources' does not exist. Make sure that the script is run from the root directory of the project, or inside 'script'.")
	input("Press enter to continue...")
	exit()

files = [f for f in files if f.endswith(".ct")]
if len(files) == 0:
	print("-- FATAL: No files ending with '.ct' found in 'resources' directory, aborting...")
	input("Press enter to continue...")
	exit()
else:
	print("-- Found ", len(files), "'*.ct' files")

print("-- Generating highlighted code in 'highlighted_codes.html'")
output = open("highlighted_codes.html", "w")
output.write('<!DOCTYPE html><html><head><meta charset="UTF-8"><title>Highlighted Code</title><script src="resources/highlight_code/highlight.js"></script><link rel="stylesheet" href="resources/highlight_code/style.css"></head><body>')
for filename in files:
	content = open("resources/{}".format(filename), "r").read()
	output.write('<h1 class="code-title">{}</h1>'.format(filename))	
	output.write('<samp><script>highlight_colt(String.raw`{}`);</script></samp>'.format(content))

output.write("</body></html>")
# Close .html file
output.close()
print("-- Closed file, generation successful")
input("Press enter to continue...")