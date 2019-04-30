result <- read.table("C:\\Users\\User1\\CLionProjects\\DP\\Data\\Results\\fcs_unlabeled_32dim_to_cluster_output_1000_1.txt", header = F)

resultIndexes <- result[,length(result)]

#need to know where original labels are
originalIndexes <- fcs_labeled_32dim@exprs[,40]

originalNAs <- which(originalIndexes %in% NaN)

resultIndexes <- resultIndexes[-originalNAs]
originalIndexes <- originalIndexes[-originalNAs]

myNoiseIndexes <- which(resultIndexes %in% -1)

resultIndex_noNoise <- resultIndexes[-myNoiseIndexes]
originalIndex_noNoise <- originalIndexes[-myNoiseIndexes]


install.packages("clues")
library(clues)
#adjustedRand(cl1, cl2, randMethod = c("Rand", "HA", "MA", "FM", "Jaccard"))
adjustedRand(originalIndex_noNoise, resultIndex_noNoise)

#optionally
library(clv)
std <- std.ext(originalIndex_noNoise, resultIndex_noNoise)
clv.Rand(std)
std