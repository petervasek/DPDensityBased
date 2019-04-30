library(ggplot2)

dyn_result_p2 <- read.table("C:\\Users\\user1\\CLionProjects\\DP\\Data\\Sources\\t4.8k second.dat", header = F)

my_theme <- theme(axis.text=element_text(size=12),
                  axis.title=element_text(size=13,face="bold"),
                  panel.background = element_rect(color = "grey25", fill = "white"),
                  panel.grid.major = element_blank(),
                  panel.grid.minor = element_blank())

plain_result_cells <- read.table("C:\\Users\\User1\\CLionProjects\\DP\\Data\\Results\\t4.8k_clustering_output_25_10.txt", header = F)
ggplot(data.frame(plain_result_cells[,c(1,2)], Class = as.character(plain_result_cells[,3])), aes(V1, V2)) +
  geom_point(aes(colour = Class, shape = Class), size = 1.8, alpha = 0.8) +
  my_theme +
  scale_colour_manual(values=c("salmon", "purple", "springgreen3", "red",
                               "orange", "brown", "pink", "dodgerblue3", "magenta",
                               "gold", "black", "darkgrey", "green", "lightblue")) +
  scale_shape_manual(values = c(15,16,17,18,19,20,15,16,17,18,19,20,15,16,17,18,19,20)) +
  labs(title = "t4.8k") +
  labs(x = "X", y = "Y")


dyn_result_fin1 <- read.table("C:\\Users\\User1\\CLionProjects\\DP\\Data\\Results\\t4.8k first_clustering_output_25_10.txt", header = F)
ggplot(data.frame(dyn_result_fin1[,c(1,2)], Class = as.character(dyn_result_fin1[,3])), aes(V1, V2)) +
  geom_point(aes(colour = Class, shape = Class), size = 1.8, alpha = 0.8) +
  my_theme +
  scale_colour_manual(values=c("salmon", "purple", "springgreen3", "red",
                               "orange", "brown", "pink", "dodgerblue3", "magenta",
                               "gold", "black", "darkgrey", "green", "lightblue")) +
  scale_shape_manual(values = c(15,16,17,18,19,20,15,16,17,18,19,20,15,16,17,18,19,20)) +
  labs(title = "t4.8k") +
  labs(x = "X", y = "Y")

dyn_result_cells_fin1 <- read.table("C:\\Users\\User1\\CLionProjects\\DP\\Data\\Results\\t4.8k first_clustering_output_cells_25_10.txt", header = F)
ggplot(data.frame(dyn_result_cells_fin1[,c(1,2)], Class = as.character(dyn_result_cells_fin1[,3])), aes(V1, V2)) +
  geom_point(aes(colour = Class, shape = Class), size = 2, alpha = 0.8) +
  my_theme +
  scale_colour_manual(values=c("salmon", "purple", "springgreen3", "red",
                               "orange", "brown", "pink", "dodgerblue3", "magenta",
                               "gold", "black", "darkgrey", "green", "lightblue")) +
  scale_shape_manual(values = c(15,16,17,18,19,20,15,16,17,18,19,20,15,16,17,18,19,20)) +
  labs(title = "t4.8k") +
  labs(x = "X", y = "Y")


dyn_result_fin2 <- read.table("C:\\Users\\User1\\CLionProjects\\DP\\Data\\Results\\t4.8k second_clustering_output_25_10.txt", header = F)
ggplot(data.frame(dyn_result_fin2[,c(1,2)], Class = as.character(dyn_result_fin2[,3])), aes(V1, V2)) +
  geom_point(aes(colour = Class, shape = Class), size = 1.8, alpha = 0.8) +
  my_theme +
  scale_colour_manual(values=c("salmon", "purple", "springgreen3", "red",
                               "orange", "brown", "pink", "dodgerblue3", "magenta",
                               "gold", "black", "grey", "green", "lightblue", "purple", "springgreen3", "red",
                               "orange", "brown", "pink", "dodgerblue3")) +
  scale_shape_manual(values = c(15,16,17,18,19,20,15,16,17,18,19,20,15,16,17,18,19,20)) +
  labs(title = "t4.8k") +
  labs(x = "X", y = "Y")


dyn_result_cells_fin2 <- read.table("C:\\Users\\User1\\CLionProjects\\DP\\Data\\Results\\t4.8k second_clustering_output_cells_25_10.txt", header = F)
ggplot(data.frame(dyn_result_cells_fin2[,c(1,2)], Class = as.character(dyn_result_cells_fin2[,3])), aes(V1, V2)) +
  geom_point(aes(colour = Class, shape = Class), size = 2, alpha = 0.8) +
  my_theme +
  scale_colour_manual(values=c("salmon", "purple", "springgreen3", "red",
                               "orange", "brown", "pink", "dodgerblue3", "magenta",
                               "gold", "black", "grey", "green", "lightblue", "purple", "springgreen3", "red",
                               "orange", "brown", "pink", "dodgerblue3", "magenta",
                               "gold", "black", "grey", "green", "lightblue", "purple", "springgreen3", "red",
                               "orange", "brown", "pink", "dodgerblue3", "magenta")) +
  scale_shape_manual(values = c(15,16,17,18,19,20,15,16,17,18,19,20,15,16,17,18,19,20)) +
  labs(title = "t4.8k") +
  labs(x = "X", y = "Y")



pca_13dim <- read.table("C:\\Users\\User1\\CLionProjects\\DP\\Viz\\pca_levine_13dim.txt", header = T)
labels <- fcs8_labeled_13dim@exprs[,14]
labels <- result_1.1_500[,14]
labelsNan <- which(labels %in% NaN)
labelsNan <- which(labels %in% -1)
pca_13dim <- pca_13dim[-labelsNan,]
labels <- labels[-labelsNan]
ggplot(data.frame(pca_13dim[,c(1,2)], Class = as.character(labels)), aes(V1, V2)) +
  geom_point(aes(colour = Class, shape = Class), size = 0.8, alpha = 1) +
  my_theme +
  scale_colour_manual(values=c("salmon", "purple", "springgreen3", "red",
                               "orange", "brown", "pink", "dodgerblue3", "magenta",
                               "gold", "black", "grey", "green", "lightblue",
                               "orange", "brown", "pink", "dodgerblue3", "magenta",
                               "gold", "black", "grey", "green", "lightblue", "salmon")) +
  scale_shape_manual(values = c(15,16,17,18,19,20,15,16,17,18,19,20,15,
                                16,17,18,19,20,17,18,19,20,15,16,17,18,19,20)) +
  labs(title = "Levine_13dim") +
  labs(x = "V1", y = "V2")


pca_32im <- read.table("C:\\Users\\User1\\CLionProjects\\DP\\Viz\\pca_levine_32dim.txt", header = T)
labels <- fcs_labeled_32dim@exprs[,40]
labels <- result1[,33]
labelsNan <- which(labels %in% NaN)
labelsNan <- which(labels %in% -1)
pca_32im <- pca_32im[-labelsNan,]
labels <- labels[-labelsNan]
ggplot(data.frame(pca_32im[,c(1,2)], Class = as.character(labels)), aes(V1, V2)) +
  geom_point(aes(colour = Class, shape = Class), size = 0.8, alpha = 1) +
  my_theme +
  scale_colour_manual(values=c("salmon", "purple", "lightblue", "red",
                               "orange", "brown", "pink", "dodgerblue3", "magenta",
                               "gold", "black", "grey", "green", "lightblue",
                               "orange", "brown", "pink", "dodgerblue3", "magenta",
                               "gold", "black", "grey", "green", "springgreen", "salmon")) +
  scale_shape_manual(values = c(15,16,17,18,19,20,15,16,17,18,19,20,15,
                                16,17,18,19,20,17,18,19,20,15,16,17,18,19,20)) +
  labs(title = "Levine_32dim") +
  labs(x = "V1", y = "V2")


pca_39dim <- read.table("C:\\Users\\User1\\CLionProjects\\DP\\Viz\\pca_samusik_39dim.txt", header = T)
labels <- samusik_original_labels
labelsNan <- which(labels %in% NaN)
pca_39dim <- pca_39dim[-labelsNan,]
labels <- labels[-labelsNan]
ggplot(data.frame(pca_39dim[,c(1,2)], Class = as.character(labels)), aes(V1, V2)) +
  geom_point(aes(colour = Class, shape = Class), size = 0.8, alpha = 1) +
  my_theme +
  scale_colour_manual(values=c("salmon", "purple", "lightblue", "red",
                               "orange", "brown", "pink", "dodgerblue3", "magenta",
                               "gold", "black", "grey", "green", "lightblue",
                               "orange", "brown", "pink", "dodgerblue3", "magenta",
                               "gold", "black", "grey", "green", "springgreen", "salmon")) +
  scale_shape_manual(values = c(15,16,17,18,19,20,15,16,17,18,19,20,15,
                                16,17,18,19,20,17,18,19,20,15,16,17,18,19,20)) +
  labs(title = "Samusik_all_39dim") +
  labs(x = "V1", y = "V2")


tsne_13dim <- read.table("C:\\Users\\User1\\CLionProjects\\DP\\Viz\\tsneLevine_32dim.txt", header = T)
labels <- fcs8_labeled_13dim@exprs[,14]
labels <- result_1.1_500[,14]
labelsNan <- which(labels %in% NaN)
labelsNan <- which(labels %in% -1)
tsne_13dim <- tsne_13dim[-labelsNan,]
labels <- labels[-labelsNan]
ggplot(data.frame(tsne_13dim[,c(1,2)], Class = as.character(labels)), aes(V1, V2)) +
  geom_point(aes(colour = Class, shape = Class), size = 0.8, alpha = 1) +
  my_theme +
  scale_colour_manual(values=c("salmon", "purple", "springgreen3", "red",
                               "orange", "brown", "pink", "dodgerblue3", "magenta",
                               "gold", "black", "grey", "green", "lightblue",
                               "orange", "brown", "pink", "dodgerblue3", "magenta",
                               "gold", "black", "grey", "green", "lightblue", "salmon")) +
  scale_shape_manual(values = c(15,16,17,18,19,20,15,16,17,18,19,20,15,
                                16,17,18,19,20,17,18,19,20,15,16,17,18,19,20)) +
  labs(title = "t-SNE Levine_13dim") +
  labs(x = "V1", y = "V2")



tsne_39dim <- read.table("C:\\Users\\User1\\CLionProjects\\DP\\Viz\\tsneSamusik_39dim.txt", header = T)
labels <- samusik_original_labels
labelsNan <- which(labels %in% NaN)
tsne_39dim <- tsne_39dim[-labelsNan,]
labels <- labels[-labelsNan]
ggplot(data.frame(tsne_39dim[,c(1,2)], Class = as.character(labels)), aes(V1, V2)) +
  geom_point(aes(colour = Class, shape = Class), size = 0.8, alpha = 1) +
  my_theme +
  scale_colour_manual(values=c("salmon", "purple", "springgreen3", "red",
                               "orange", "brown", "pink", "dodgerblue3", "magenta",
                               "gold", "black", "grey", "green", "lightblue",
                               "orange", "brown", "pink", "dodgerblue3", "magenta",
                               "gold", "black", "grey", "green", "lightblue", "salmon")) +
  scale_shape_manual(values = c(15,16,17,18,19,20,15,16,17,18,19,20,15,
                                16,17,18,19,20,17,18,19,20,15,16,17,18,19,20)) +
  labs(title = "t-SNE Samusik_all_39dim") +
  labs(x = "V1", y = "V2")
