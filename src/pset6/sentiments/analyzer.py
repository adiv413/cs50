import nltk

class Analyzer():
    """Implements sentiment analysis."""

    def __init__(self, positives, negatives):
        self.positives = set()
        lines = open(positives, "r")
        for line in lines:
            if line.startswith(';') == False:
                self.positives.add(line.rstrip("\n"))
        lines.close()

        self.negatives = set()
        lines = open(negatives, "r")
        for line in lines:
            if line.startswith(';') == False:
                self.negatives.add(line.rstrip("\n"))
        lines.close()

    def analyze(self, text):
        """Analyze text for sentiment, returning its score."""
        tokenizer = nltk.tokenize.TweetTokenizer()
        tokens = tokenizer.tokenize(text)
        sum = 0
        for word in tokens:
            if word.lower() in self.positives:
                sum += 1
            elif word.lower() in self.negatives:
                sum -= 1
            else:
                continue

        return sum