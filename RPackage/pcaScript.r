install.packages("rARPACK")
library(rARPACK)

pc <- function(x, k) {
     # First center data
     xc <- scale(x, center = TRUE, scale = TRUE)
     # Partial SVD
     decomp <- svds(xc, k, nu = 0, nv = k)
     return(xc %*% decomp$v)
}

pca_levine_13dim <- pc(result_13dim, 2)
pca_levine_32dim <- pc(result_32dim, 2)
pca_samusik_39dim <- pc(result_samusik_39dim, 2)
