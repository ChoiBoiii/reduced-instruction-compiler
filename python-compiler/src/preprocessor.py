from pathlib import Path
from typing import Union
from time import strftime


class Preprocessor:

    ## CONFIG ...
    
    ## Comment identification
    START_ML_COMMENT = "/*" # The substr used to define the start of a single line comment
    END_ML_COMMENT   = "*/" # The substr used to define the start of a multiline comment
    START_SL_COMMENT = "//" # The substr used to define the end of a multiline comment

    ## Literal field delimiters
    STR_LITERAL_DELIMITER = '"'
    CHAR_LITERAL_DELIMITER = "'"

    ## Directive delimiters
    DIRECTIVE_DELIMITER = '#'


    ## INITIALISERS ...

    ## INITIALISE PREPROCESSOR INSTANCE
    def __init__(self, fileName: str, includePaths: list[str]=[]):

        ## Save source file path
        self.sourcePath = Path(fileName)

        ## Save include paths 
        self.includePaths = includePaths

        ## Stack used internally to track file currently being processed. Used for __FILE__ macro
        self.__currentFile = []

        ## Construct directive dict for use when processing directives
        # self.directives = dict({})
        # self.directives[Preprocessor.DIRECTIVE_DELIMITER + "include"] = 
        # self.directives[Preprocessor.DIRECTIVE_DELIMITER + "define"]

        ## Construct macro dict for use when preprocessing
        ## TODO - Make each macro a function that returns a string?
        self.macros = dict({})
        # ANSI C defined macros
        self.macros["__DATE__"] = lambda : '"' + strftime('%b %d %Y') + '"'
        self.macros["__TIME__"] = lambda : '"' + strftime('%H:%M:%S') + '"'
        self.macros["__FILE__"] = lambda : '"' + self.__currentFile[-1].name + '"'
        self.macros["__STDC__"] = lambda : '1'

        ## Get processed text
        self.processed = self.process_file(self.sourcePath)
        
    
    ## METHODS ...

    ## Remove all comments from the given text
    @staticmethod
    def strip_comments(text: str) -> str:

        ## Save valid text to output
        out = ""

        ## Add a newline to end of text for compatibility
        text += '\n'

        ## Iterate text per character
        slComment   = False # Whether currently inside a single line comment
        mlComment   = False # Whether currently inside a multi line comment
        strLiteral  = False # Whether currently inside a string literal
        charLiteral = False # Whether currently inside a char literal
        startIndex  = 0     # Start index of valid substr to copy to output 
        for i, c in enumerate(text):

            ## Currently inside a char literal
            if charLiteral:
                if c == Preprocessor.CHAR_LITERAL_DELIMITER:
                    charLiteral = False
                
            ## Currently inside a string literal
            elif strLiteral:
                if c == Preprocessor.STR_LITERAL_DELIMITER:
                    strLiteral = False

            ## Currently inside a single line comment
            elif slComment:
                if c == '\n':
                    slComment = False
                    startIndex = i

            ## Currently inside a multiline comment
            elif mlComment:
                if (c == Preprocessor.END_ML_COMMENT[0]) and (text[i:i+len(Preprocessor.END_ML_COMMENT)] == Preprocessor.END_ML_COMMENT):
                    mlComment = False
                    startIndex = i + len(Preprocessor.END_ML_COMMENT)

            ## Not in any escape sequence
            else:
                
                ## Begin char literal field
                if c == Preprocessor.CHAR_LITERAL_DELIMITER:
                    charLiteral = True

                ## Begin str literal field
                elif c == Preprocessor.STR_LITERAL_DELIMITER:
                    strLiteral = True
                
                ## Begin single line comment
                elif (c == Preprocessor.START_SL_COMMENT[0]) and (text[i:i+len(Preprocessor.START_SL_COMMENT)] == Preprocessor.START_SL_COMMENT):
                        slComment = True
                        out += text[startIndex:i]

                ## Begin multiline comment
                elif (c == Preprocessor.START_ML_COMMENT[0]) and (text[i:i+len(Preprocessor.START_ML_COMMENT)] == Preprocessor.START_ML_COMMENT):
                        mlComment = True
                        out += text[startIndex:i]

        ## Add remaining valid text
        out += text[startIndex:]

        ## Detect error: Unterminated comment
        if mlComment:
            raise SyntaxError(f"Unterminated /* comment")

        ## Return
        return out


    ## Processes the given file. Recursively calls self on each occurance of a #include
    def process_file(self, filePath: Union[str, Path]) -> str:

        ## Read in file
        path = Path(filePath) if type(filePath) == str else filePath
        with open(path, 'r', encoding='unicode_escape') as f:
            source = f.read()
        
        ## Add current file to stack
        self.__currentFile.append(path)

        ## Strip comments
        source = Preprocessor.strip_comments(source)

        ## Process directives by iterating and replacing lines
        source = source.split('\n')
        for i, line in enumerate(source):

            ## If line is a preprocessor directive
            tmpLine = line.split()
            if tmpLine:
                if tmpLine[0][0] == Preprocessor.DIRECTIVE_DELIMITER:

                    ## Get directive
                    if tmpLine[0] == Preprocessor.DIRECTIVE_DELIMITER:
                        directive = tmpLine[0] + tmpLine[1]
                        args = tmpLine[2:]
                    else:
                        directive = tmpLine[0]
                        args = tmpLine[1:]

                    ## Handle #include
                    if directive == "#include":
                        try:
                            fPath = args[0][1:-1]
                            source[i] = self.process_file(fPath)
                        except Exception as e:
                            print("ERROR: Invalid #include statement")
                            raise e

        ## Remove current file from stack
        self.__currentFile.pop()

        ## Re-join lines into single string
        source = '\n'.join(source)

        ## Return processed text
        return source

