#******************************************************************
#
# ---------------- Aggregates time plots --------------------
#
#******************************************************************

# remove warnings for support functions
# !diagnostics suppress = log0, plot_lists, hpfilter, colSds, colMins, colMaxs


time_plots <- function( mcData, Adata, mdata, Mdata, Sdata, nExp, nSize, nTsteps,
                        TmaskPlot, CI, legends, colors, lTypes, transMk, smoothing,
                        firmTypes ) {

  
  # ------ Orders, Production and Capacity ------
  
  exps <- min <- max <- lo <- hi <- list( )
  # select data to plot
  for( k in 1 : nExp ){
    # MC averages
    exps[[ k ]] <- list( log0( Adata[[ k ]]$Sector_Effective_Production[ TmaskPlot ] ),
                         log0( Adata[[ k ]]$Sector_Effective_Orders[ TmaskPlot ] ),
                         log0( Adata[[ k ]]$Sector_Productive_Capacity[ TmaskPlot ] ) )
    # minimum and maximum MC runs
    min[[ k ]] <- list( log0( mdata[[ k ]]$Sector_Effective_Production[ TmaskPlot ] ),
                        log0( mdata[[ k ]]$Sector_Effective_Orders[ TmaskPlot ] ),
                        log0( mdata[[ k ]]$Sector_Productive_Capacity[ TmaskPlot ] ) )
    max[[ k ]] <- list( log0( Mdata[[ k ]]$Sector_Effective_Production[ TmaskPlot ] ),
                        log0( Mdata[[ k ]]$Sector_Effective_Orders[ TmaskPlot ] ),
                        log0( Mdata[[ k ]]$Sector_Productive_Capacity[ TmaskPlot ] ) )
    # MC confidence interval
    lo[[ k ]] <- list( log0( Adata[[ k ]]$Sector_Effective_Production[ TmaskPlot ] -
                               qnorm(1 - (1 - CI ) / 2) * Sdata[[ k ]]$Sector_Effective_Production[ TmaskPlot ] / sqrt( nSize ) ),
                       log0( Adata[[ k ]]$Sector_Effective_Orders[ TmaskPlot ] -
                               qnorm(1 - (1 - CI ) / 2) * Sdata[[ k ]]$Sector_Effective_Orders[ TmaskPlot ] / sqrt( nSize ) ),
                       log0( Adata[[ k ]]$Sector_Productive_Capacity[ TmaskPlot ] -
                               qnorm(1 - (1 - CI ) / 2) * Sdata[[ k ]]$Sector_Productive_Capacity[ TmaskPlot ]  / sqrt( nSize ) ) )
    hi[[ k ]] <- list( log0( Adata[[ k ]]$Sector_Effective_Production[ TmaskPlot ] +
                               qnorm(1 - (1 - CI ) / 2) * Sdata[[ k ]]$Sector_Effective_Production[ TmaskPlot ] / sqrt( nSize ) ),
                       log0( Adata[[ k ]]$Sector_Effective_Orders[ TmaskPlot ] +
                               qnorm(1 - (1 - CI ) / 2) * Sdata[[ k ]]$Sector_Effective_Orders[ TmaskPlot ] / sqrt( nSize ) ),
                       log0( Adata[[ k ]]$Sector_Productive_Capacity[ TmaskPlot ] +
                               qnorm(1 - (1 - CI ) / 2) * Sdata[[ k ]]$Sector_Productive_Capacity[ TmaskPlot ]  / sqrt( nSize ) ) )
  }
  
  plot_lists( exps, min, max, lo, hi, leg = legends, mrk = transMk, col = colors,
              lty = lTypes, xlab = "Time", ylab = "Logs",
              tit = "Production, Orders and Capacity",
              subtit = paste( "MC runs =", nSize ),
              leg2 = c( "Production", "Orders", "Capacity" ) )
  
  # ------ Sector Income  ------
  
  exps <- min <- max <- lo <- hi <- list( )
  # select data to plot
  for( k in 1 : nExp ){
    # MC averages
    exps[[ k ]] <- list( log0( Adata[[ k ]]$Sector_Income[ TmaskPlot ] ))
    # minimum and maximum MC runs
    min[[ k ]] <- list( log0( mdata[[ k ]]$Sector_Income[ TmaskPlot ] ))
    max[[ k ]] <- list( log0( Mdata[[ k ]]$Sector_Income[ TmaskPlot ] ))
    # MC confidence interval
    lo[[ k ]] <- list( log0( Adata[[ k ]]$Sector_Income[ TmaskPlot ] -
                               qnorm(1 - (1 - CI ) / 2) * Sdata[[ k ]]$Sector_Income[ TmaskPlot ] / sqrt( nSize ) ))
    hi[[ k ]] <- list( log0( Adata[[ k ]]$Sector_Income[ TmaskPlot ] +
                               qnorm(1 - (1 - CI ) / 2) * Sdata[[ k ]]$Sector_Income[ TmaskPlot ] / sqrt( nSize ) ))
  }
  
  plot_lists( exps, min, max, lo, hi, leg = legends, mrk = transMk, col = colors,
              lty = lTypes, xlab = "Time", ylab = "Log",
              tit = "Nominal Income",
              subtit = paste( "MC runs =", nSize ) )
  
  
  # ------ Income Growth ------

  exps <- min <- max <- lo <- hi <- list( )
  # select data to plot
  for( k in 1 : nExp ){
    # MC averages
    exps[[ k ]] <- list( Adata[[ k ]]$Sector_Income_Growth[ TmaskPlot ] )
    # minimum and maximum MC runs
    min[[ k ]] <- list( mdata[[ k ]]$Sector_Income_Growth[ TmaskPlot ])
    max[[ k ]] <- list( Mdata[[ k ]]$Sector_Income_Growth[ TmaskPlot ])
    # MC confidence interval
    lo[[ k ]] <- list( Adata[[ k ]]$Sector_Income_Growth[ TmaskPlot ] -
                       qnorm(1 - (1 - CI ) / 2) * Sdata[[ k ]]$Sector_Income_Growth[ TmaskPlot ] / sqrt( nSize ))
    hi[[ k ]] <- list( Adata[[ k ]]$Sector_Income_Growth[ TmaskPlot ] +
                       qnorm(1 - (1 - CI ) / 2) * Sdata[[ k ]]$Sector_Income_Growth[ TmaskPlot ] / sqrt( nSize ))
  }

  plot_lists( exps, min, max, lo, hi, leg = legends, mrk = transMk, col = colors,
              lty = lTypes, xlab = "Time", ylab = "Rate",
              tit = "Income Growth",
              subtit = paste( "MC runs =", nSize ),
              leg2 = c( "Income Growth") )
  
  # ------ Wages and Profits ------
  
  exps <- min <- max <- lo <- hi <- list( )
  # select data to plot
  for( k in 1 : nExp ){
    # MC averages
    
    exps[[ k ]] <- list( log0( Adata[[ k ]]$Sector_Wages[ TmaskPlot ] ), 
                         log0( Adata[[ k ]]$Sector_Profits[ TmaskPlot ] ))
    
    # minimum and maximum MC runs
    
    min[[ k ]] <- list( log0( mdata[[ k ]]$Sector_Wages[ TmaskPlot ] ),
                        log0( mdata[[ k ]]$Sector_Profits[ TmaskPlot ] ) )
    
    max[[ k ]] <- list( log0( Mdata[[ k ]]$Sector_Wages[ TmaskPlot ] ),
                        log0( Mdata[[ k ]]$Sector_Profits[ TmaskPlot ] ))
    
    # MC confidence interval
    
    lo[[ k ]] <- list( log0( Adata[[ k ]]$Sector_Wages[ TmaskPlot ] -
                               qnorm(1 - (1 - CI ) / 2) * Sdata[[ k ]]$Sector_Wages[ TmaskPlot ] / sqrt( nSize ) ),
                       log0( Adata[[ k ]]$Sector_Profits[ TmaskPlot ] -
                               qnorm(1 - (1 - CI ) / 2) * Sdata[[ k ]]$Sector_Profits[ TmaskPlot ] / sqrt( nSize ) ))
    
    hi[[ k ]] <- list( log0( Adata[[ k ]]$Sector_Wages[ TmaskPlot ] +
                               qnorm(1 - (1 - CI ) / 2) * Sdata[[ k ]]$Sector_Wages[ TmaskPlot ] / sqrt( nSize ) ),
                       log0( Adata[[ k ]]$Sector_Profits[ TmaskPlot ] +
                               qnorm(1 - (1 - CI ) / 2) * Sdata[[ k ]]$PROFITS[ TmaskPlot ] / sqrt( nSize ) ))
  }
  
  plot_lists( exps, min, max, lo, hi, leg = legends, mrk = transMk, col = colors,
              lty = lTypes, xlab = "Time", ylab = "Logs",
              tit = "Wages and Profits",
              subtit = paste( "MC runs =", nSize ),
              leg2 = c( "Wages", "Profits" ) )
  
  
  # ------ Sector Price, Wages and Productivity ------
  
  exps <- min <- max <- lo <- hi <- list( )
  # select data to plot
  for( k in 1 : nExp ){
    # MC averages
    exps[[ k ]] <- list( log0( Adata[[ k ]]$Sector_Avg_Price[ TmaskPlot ] ),
                         log0( Adata[[ k ]]$Sector_Avg_Wage[ TmaskPlot ] ),
                         log0( Adata[[ k ]]$Sector_Avg_Productivity[ TmaskPlot ] ) )
    # minimum and maximum MC runs
    min[[ k ]] <- list( log0( mdata[[ k ]]$Sector_Avg_Price[ TmaskPlot ] ),
                        log0( mdata[[ k ]]$Sector_Avg_Wage[ TmaskPlot ] ),
                        log0( mdata[[ k ]]$Sector_Avg_Productivity[ TmaskPlot ] ) )
    max[[ k ]] <- list( log0( Mdata[[ k ]]$Sector_Avg_Price[ TmaskPlot ] ),
                        log0( Mdata[[ k ]]$Sector_Avg_Wage[ TmaskPlot ] ),
                        log0( Mdata[[ k ]]$Sector_Avg_Productivity[ TmaskPlot ] ) )
    # MC confidence interval
    lo[[ k ]] <- list( log0( Adata[[ k ]]$Sector_Avg_Price[ TmaskPlot ] -
                               qnorm(1 - (1 - CI ) / 2) * Sdata[[ k ]]$Sector_Avg_Price[ TmaskPlot ] / sqrt( nSize ) ),
                       log0( Adata[[ k ]]$Sector_Avg_Wage[ TmaskPlot ] -
                               qnorm(1 - (1 - CI ) / 2) * Sdata[[ k ]]$Sector_Avg_Wage[ TmaskPlot ] / sqrt( nSize ) ),
                       log0( Adata[[ k ]]$Sector_Avg_Productivity[ TmaskPlot ] -
                               qnorm(1 - (1 - CI ) / 2) * Sdata[[ k ]]$Sector_Avg_Productivity[ TmaskPlot ]  / sqrt( nSize ) ) )
    hi[[ k ]] <- list( log0( Adata[[ k ]]$Sector_Avg_Price[ TmaskPlot ] +
                               qnorm(1 - (1 - CI ) / 2) * Sdata[[ k ]]$Sector_Avg_Price[ TmaskPlot ] / sqrt( nSize ) ),
                       log0( Adata[[ k ]]$Sector_Avg_Wage[ TmaskPlot ] +
                               qnorm(1 - (1 - CI ) / 2) * Sdata[[ k ]]$Sector_Avg_Wage[ TmaskPlot ] / sqrt( nSize ) ),
                       log0( Adata[[ k ]]$Sector_Avg_Productivity[ TmaskPlot ] +
                               qnorm(1 - (1 - CI ) / 2) * Sdata[[ k ]]$Sector_Avg_Productivity[ TmaskPlot ]  / sqrt( nSize ) ) )
  }
  
  plot_lists( exps, min, max, lo, hi, leg = legends, mrk = transMk, col = colors,
              lty = lTypes, xlab = "Time", ylab = "Logs",
              tit = "Price, Wages and Prouctivity (Average)",
              subtit = paste( "MC runs =", nSize ),
              leg2 = c( "Price", "Wages", "Productivity" ) )
  
  # ------ Inflation ------
  
  exps <- min <- max <- lo <- hi <- list( )
  # select data to plot
  for( k in 1 : nExp ){
    # MC averages
    exps[[ k ]] <- list( Adata[[ k ]]$Sector_Inflation[ TmaskPlot ] )
    # minimum and maximum MC runs
    min[[ k ]] <- list( mdata[[ k ]]$Sector_Inflation[ TmaskPlot ])
    max[[ k ]] <- list( Mdata[[ k ]]$Sector_Inflation[ TmaskPlot ])
    # MC confidence interval
    lo[[ k ]] <- list( Adata[[ k ]]$Sector_Inflation[ TmaskPlot ] -
                         qnorm(1 - (1 - CI ) / 2) * Sdata[[ k ]]$Sector_Inflation[ TmaskPlot ] / sqrt( nSize ))
    hi[[ k ]] <- list( Adata[[ k ]]$Sector_Inflation[ TmaskPlot ] +
                         qnorm(1 - (1 - CI ) / 2) * Sdata[[ k ]]$Sector_Inflation[ TmaskPlot ] / sqrt( nSize ))
  }
  
  plot_lists( exps, min, max, lo, hi, leg = legends, mrk = transMk, col = colors,
              lty = lTypes, xlab = "Time", ylab = "Rate",
              tit = "Inflation",
              subtit = paste( "MC runs =", nSize ) )
  
  
  # ------ Profit Rate  ------
  
  exps <- min <- max <- lo <- hi <- list( )
  # select data to plot
  for( k in 1 : nExp ){
    # MC averages
    exps[[ k ]] <- list( Adata[[ k ]]$Sector_Profit_Rate[ TmaskPlot ] )
    # minimum and maximum MC runs
    min[[ k ]] <- list( mdata[[ k ]]$Sector_Profit_Rate[ TmaskPlot ] )
    max[[ k ]] <- list( Mdata[[ k ]]$Sector_Profit_Rate[ TmaskPlot ] )
    # MC confidence interval
    lo[[ k ]] <- list( Adata[[ k ]]$Sector_Profit_Rate[ TmaskPlot ] -
                         qnorm(1 - (1 - CI ) / 2) * Sdata[[ k ]]$Sector_Profit_Rate[ TmaskPlot ] / sqrt( nSize ) )
    hi[[ k ]] <- list( Adata[[ k ]]$Sector_Profit_Rate[ TmaskPlot ] +
                         qnorm(1 - (1 - CI ) / 2) * Sdata[[ k ]]$Sector_Profit_Rate[ TmaskPlot ] / sqrt( nSize ) )
  }
  
  plot_lists( exps, min, max, lo, hi, leg = legends, mrk = transMk, col = colors,
              lty = lTypes, xlab = "Time", ylab = "Rate",
              tit = "Profit Rate",
              subtit = paste( "MC runs =", nSize ) )
  
  # ------ Capacity Utilization  ------
  
  exps <- min <- max <- lo <- hi <- list( )
  # select data to plot
  for( k in 1 : nExp ){
    # MC averages
    exps[[ k ]] <- list( Adata[[ k ]]$Sector_Capacity_Utilization[ TmaskPlot ] )
    # minimum and maximum MC runs
    min[[ k ]] <- list( mdata[[ k ]]$Sector_Capacity_Utilization[ TmaskPlot ] )
    max[[ k ]] <- list( Mdata[[ k ]]$Sector_Capacity_Utilization[ TmaskPlot ] )
    # MC confidence interval
    lo[[ k ]] <- list( Adata[[ k ]]$Sector_Capacity_Utilization[ TmaskPlot ] -
                         qnorm(1 - (1 - CI ) / 2) * Sdata[[ k ]]$Sector_Capacity_Utilization[ TmaskPlot ] / sqrt( nSize ) )
    hi[[ k ]] <- list( Adata[[ k ]]$Sector_Capacity_Utilization[ TmaskPlot ] +
                         qnorm(1 - (1 - CI ) / 2) * Sdata[[ k ]]$Sector_Capacity_Utilization[ TmaskPlot ] / sqrt( nSize ) )
  }
  
  plot_lists( exps, min, max, lo, hi, leg = legends, mrk = transMk, col = colors,
              lty = lTypes, xlab = "Time", ylab = "Rate",
              tit = "Capacity Utilization",
              subtit = paste( "MC runs =", nSize ) )

  
  # ------ Sector HHI ------
  
  exps <- min <- max <- lo <- hi <- list( )
  # select data to plot
  for( k in 1 : nExp ){
    # MC averages
    exps[[ k ]] <- list( Adata[[ k ]]$Sector_Normalized_HHI[ TmaskPlot ] )
    # minimum and maximum MC runs
    min[[ k ]] <- list( mdata[[ k ]]$Sector_Normalized_HHI[ TmaskPlot ]  )
    max[[ k ]] <- list( Mdata[[ k ]]$Sector_Normalized_HHI[ TmaskPlot ] )
    # MC confidence interval
    lo[[ k ]] <- list( Adata[[ k ]]$Sector_Normalized_HHI[ TmaskPlot ] -
                         qnorm(1 - (1 - CI ) / 2) * Sdata[[ k ]]$Sector_Normalized_HHI[ TmaskPlot ] / sqrt( nSize  ) )
    hi[[ k ]] <- list( Adata[[ k ]]$Sector_Normalized_HHI[ TmaskPlot ] +
                         qnorm(1 - (1 - CI ) / 2) * Sdata[[ k ]]$Sector_Normalized_HHI[ TmaskPlot ] / sqrt( nSize  ) )
  }
  
  plot_lists( exps, min, max, lo, hi, leg = legends, mrk = transMk, col = colors,
              lty = lTypes, xlab = "Time", ylab = "Value",
              tit = "Normalized HHI",
              subtit = paste( "MC runs =", nSize ) )
  
 
  

}
