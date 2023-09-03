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
                pass
                
            ## Currently inside a string literal
            elif strLiteral:
                pass
            
            ## Currently inside a multiline comment
            elif mlComment:
                pass
            
            ## Currently inside a single line comment
            elif slComment:
                pass

            ## Not in any escape sequence
            else:
                pass


            ## Handle string literal
            if not charLiteral:
                if c == '"':
                    strLiteral = not strLiteral

            ## Handle char literal
            if not strLiteral:
                if c == "'":
                    charLiteral = not charLiteral

            ## Handle single line comments
            if not mlComment:
                if slComment:
                    ## Whether the current char is an line end
                    if c == '\n':
                        if slComment:
                            slComment = False
                            startIndex = i
                else:
                    ## Begin single line comment
                    if c == Preprocessor.START_SL_COMMENT[0]:
                        if text[i:i+len(Preprocessor.START_SL_COMMENT)] == Preprocessor.START_SL_COMMENT:
                            slComment = True
                            out += text[startIndex:i]

            ## Handle multi line comments
            if not slComment:
                if mlComment:
                    ## End multiline comment occured
                    if c == Preprocessor.END_ML_COMMENT[0]:
                        if text[i:i+len(Preprocessor.END_ML_COMMENT)] == Preprocessor.END_ML_COMMENT:
                            mlComment = False
                            startIndex = i + len(Preprocessor.END_ML_COMMENT)
                else:
                    ## Begin multiline comment occured
                    if c == Preprocessor.START_ML_COMMENT[0]:
                        if text[i:i+len(Preprocessor.START_ML_COMMENT)] == Preprocessor.START_ML_COMMENT:
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



