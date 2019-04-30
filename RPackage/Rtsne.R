result_13dim <- read.table("C:\\Users\\test\\DP\\CLION PROJECT\\Data\\Sources\\fcs8_unlabeled.txt", header = T)
result <- result[,-length(result)]

result_u <- unique(result_13dim)

install.packages("Rtsne")
library(Rtsne)
tsne_result_13dim <- Rtsne::Rtsne(result_u)