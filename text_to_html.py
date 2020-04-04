# helper script to convert text to html, using rst2html from docutils !! 
# will be conected with the main script...

# https://docs.python.org/3/library/pathlib.html
from pathlib import Path

# https://pypi.org/project/docutils/
import docutils.io, docutils.core

def rst2html(source_path):
    # mostly taken from
    # https://github.com/getpelican/pelican/
    pub = docutils.core.Publisher(
    source_class=docutils.io.FileInput,
    destination_class=docutils.io.StringOutput)
    pub.set_components('standalone', 'restructuredtext', 'html')
    pub.process_programmatic_settings(None, None, None)
    pub.set_source(source_path=str(source_path))
    pub.publish()

    html = pub.writer.parts['whole']

    return html

def to_html(file_name): 
    DST_DIR = Path('.')

    # file_name is file we need to convert from  browse !!
    html = rst2html(file_name)
   
    return html		
    #  with open(DST_DIR / (file_name[:-4] + '.html'), 'w') as f:
    #   f.write(html)

if __name__ == '__main__':
    to_html('plain_text.txt')
