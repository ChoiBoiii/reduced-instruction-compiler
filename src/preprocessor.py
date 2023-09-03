class Preprocessor:

    ## CONFIG ...

    ## Preprocessor tokens
    TOKENS = ["include",
              "define",
              "if",
              "else",
              "elif",
              "fi",
              "endif",
              "ifdef",
              "ifndef",
              ]
    
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
        self.sourcePath = fileName

        ## Save include paths 
        self.includePaths = includePaths

        ## Read in text
        with open(fileName, 'r', encoding='unicode_escape') as f:
            self.source = f.read()

        ## Strip comments
        self.strippedComments = Preprocessor.strip_comments(self.source[:])
        
    
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



    #if defined(...) -> #ifdef
    #if !defined(...) -> #ifndef
    
    ## Handle #include

    ## Handle backslashes at EOL

    ## Handle #define


