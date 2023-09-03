from pathlib import Path
from typing import Union


class Preprocessor:

    ## CONFIG ...
    
    ## Comment identification
    START_ML_COMMENT = "/*" # The substr used to define the start of a single line comment
    END_ML_COMMENT   = "*/" # The substr used to define the start of a multiline comment
    START_SL_COMMENT = "//" # The substr used to define the end of a multiline comment

    ## Literal field delimiters
    STR_LITERAL_DELIMITER = '"'
    CHAR_LITERAL_DELIMITER = "'"


    ## INITIALISERS ...

    ## INITIALISE PREPROCESSOR INSTANCE
    def __init__(self, fileName: str, includePaths: list[str]=[]):

        ## Save source file path
        self.sourcePath = Path(fileName)

        ## Save include paths 
        self.includePaths = includePaths

        ## Construct macos dict
        defaultMacros = dict([])
        # ANSI C defined macros
        defaultMacros["__DATE__"] = self.sourcePath.name ##TODO - Change to update on a per call basis, allowing for difference between files
        print(defaultMacros["__DATE__"])
        # defaultMacros["__TIME__"] = 
        # defaultMacros["__FILE__"] = 
        # defaultMacros["__LINE__"] = 
        # defaultMacros["__STDC__"] = 

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

    ## Handle all '#' commands
    @staticmethod
    def handle_directives(text: str, includePaths: list[str]=[]) -> str:
        pass

    ## Returns the file name of the file currently being processed
    @staticmethod
    def get_current_file_name():
        pass

    ## Handles the current directive, updating the macro set as required
    def handle_directive():
        pass

    ## Processes the given file. Recursively calls self on each occurance of a #include
    def process_file(self, filePath: Union[str, Path]) -> str:

        ## Read in file
        path = Path(filePath)
        with open(path, 'r', encoding='unicode_escape') as f:
            source = f.read()
        
        ## Strip comments
        source = Preprocessor.strip_comments(source)

        ## Process directives
        source = source.split('\n')
        for line in source:
            print(line)

        ## Return processed text
        return source

