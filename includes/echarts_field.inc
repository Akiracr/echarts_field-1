<?php


/**
* Created by Randall Sibaja R.
**/
function echarts_field_get_data_from_json($key, $json, $series_x)
{
 
  $output = array();
  
  foreach($json->values as $row){
    if(in_array($row->$key, $series_x ))
    {
      $output[] = $row->$key;  
    }
  }
  return $output;
}

/**
* Created by Randall Sibaja R.
**/
function echarts_field_create_polar_radar($key, $json, $series_x, $max_y)
{

  $output = array();
  foreach($json->values as $row){
    if(in_array($row->$key, $series_x ))
    {
      $output[] = array(
        'text' => t($row->$key),
        'max' =>  intval($max_y)
      ); 
    }
  }
  return $output;
}


/**
* Created by Randall Sibaja R.
**/
function echarts_field_get_series($xaxis, $series, $data, $graph_type, $series_x)
{

  $output = array();

  if( ($graph_type == 'pie') || ($graph_type == 'funnel') ){
    foreach($data->values as $row){
      if(in_array($row->$xaxis, $series)){
        $output[] = array(
          'name' => t($row->$xaxis),
          'value' => str_replace(',', '.', $row->$series_x),
        );
      }
    }

  }else if ( ($graph_type == 'bar') || ($graph_type == 'line') ){
      
    foreach ($series as $key => $v){
    
      $data_temp = array();
      
      foreach($data->values as $row){
        if (in_array($row->$xaxis,$series_x)) 
        {
          $data_temp[] = intval(str_replace(',', '.', $row->$v));
        }
      }

      $average = array(
        'data' => array(
          array(
          'type' => 'average',
          'name'=> t('Promedio')
          )
        )
      );

     $mark =  array(
        'data' => array(
          array(
            'type' => 'max',
            'name'=> 'Maximo'
          ),
          array(
            'type' => 'min',
            'name' => 'Minimo'
          )
        )
      );


      $output[] = array(
        'name' => $v,
        'type'=> $graph_type,
        'markLine' => $average,
        'markPoint' => $mark,
        'data'=> $data_temp
      );

    }
  }elseif ($graph_type == 'radar') {
    
    foreach ($series as $key => $v) 
      {
          $data_temp = array();
          foreach($data->values as $myValue)
          {
            $data_temp[] = intval(str_replace(',', '.',$myValue-> $v));
          }
          $output[] = array('value'=> $data_temp, 'name'=> $v);
      }
  }elseif ($graph_type == 'treemap') {
            
      foreach($data->values as $row)
      {
        if(in_array($row->$xaxis, $series))
        {
          $output[] = array('name'=> $row->$xaxis , 'value' => intval($row->$series_x));
        }
      }  
  }
  return $output;  
}

/**
* Created by  Randall Sibaja R.
**/
function echarts_field_generate_settings_bar_line_graph($json, $xaxis, $series, $type, $series_x ,$title = '')
{
  
  $output = array(
    'chartType' => $type,
    'useType' => array($type),
    'containerId' => 'barline_chart',
    'options' => array(
        'tooltip' => array('trigger' => 'axis'),          
        'title' => array('text' => $title),
        'legend' => array(
          'data' => $series,
          'orient' => 'horizontal',
        ),
      'toolbox' => array(
            'show' => TRUE,
            'feature' => array(
              'mark' => array(
                'show' => TRUE,
                'title' => array(
                  'mark' => t('Marcar'),
                  'markUndo' => t('Deshacer'),
                  'markClear' => t('Limpiar Marcas')
                )
              ),
              'dataView' => array(
              'show' => TRUE,
              'readOnly' => false,
              'title' => t('Ver Datos'),
              'lang' => array(
                t('Datos'),
                t('Regresar'),
                ''
              )          
            ),
            'restore' => array(
              'show' => TRUE,
              'title' => t('Refrescar')
            ),
            'saveAsImage' => array(
              'show' => TRUE,
              'title' => t('Guardar'),
              'type' => 'png'
            ),
          )
        ),
        'dataZoom' => array(
            'show' => true,
            'realtime' => true,
            'start' => 10,
            'end' => 100
        ),
      'calculable' => true,
      'xAxis' => array(
          '0' => array(
            'type' => 'category',
            'data' => echarts_field_get_data_from_json($xaxis, $json, $series_x)
          )
      ),
      'yAxis' => array(
        '0' => array(
          'type' => 'value'
        )
      ),
      'series' => echarts_field_get_series($xaxis, $series, $json, $type, $series_x),
    )
  );

  return $output;
}

/**
* Created by Randall Sibaja R.
**/
function echarts_field_generate_settings_pie_funnel_graph($json,$xaxis , $series, $type, $series_x, $title='')
{
    
  $chart = array(
    'chartType' => $type,
    'useType' => array($type),
    'containerId' => 'pie_chart',
    'options' => array(
        'tooltip' => array(
          'trigger' => 'item',
          'formatter' => '{a} <br/>{b} : {c} ({d}%)'
        ),      
        'title' => array(
          'text' => $title,
          'subtext' => $series_x,
          'x'=> 'center'
        ),
        'legend' => array(
          'data' => $series,
          'orient' => 'horizontal',
          'x' => 'left',
          'y' => 'bottom'
        ),
          'toolbox' => array(
            'show' => TRUE,
            'feature' => array(
              'mark' => array(
                'show' => TRUE,
                'title' => array(
                  'mark' => t('Marcar'),
                  'markUndo' => t('Deshacer'),
                  'markClear' => t('Limpiar Marcas')
                )
              ),
              'dataView' => array(
              'show' => TRUE,
              'readOnly' => false,
              'title' => t('Ver Datos'),
              'lang' => array(
                t('Datos'),
                t('Regresar'),
                ''
              )          
            ),
            'restore' => array(
              'show' => TRUE,
              'title' => t('Refrescar')
            ),
            'saveAsImage' => array(
              'show' => TRUE,
              'title' => t('Guardar'),
              'type' => 'png'
            ),
          )
        ),
        'calculable' => TRUE,
        'series' => array(
          array(
              'name'=> t($series_x),
              'type'=> $type,
              'width' => $type == 'funnel' ? '50%' : '80%',
              'x' => $type == 'funnel' ? '5%': '',
              'radius' => '70%',
              'funnelAlign' => $type == 'funnel' ? 'left' : '',   
              'center' => array('50%','60%'),
              'data'=> echarts_field_get_series($xaxis, $series, $json, $type, $series_x)
          )          
        )
      )
    );
  return $chart;
}

/**
*Created by  Randall Sibaja R.
*
**/
function echarts_field_generate_settings_radar_graph($json, $xaxis , $series, $type, $series_x, $max_y, $title='')
{  
  $radar = array(
    'chartType' => $type,
    'useType' => array($type),
    'containerId' => 'radar_chart',
    'options' => array(
        'tooltip' => array('trigger' => 'axis'),          
        'title' => array('text' => $title),
        'legend' => array(
          'data' => $series,
          'orient' => 'horizontal',
          'x' => 'center',
          'y' => 'bottom'
        ),
         'toolbox' => array(
            'show' => TRUE,
            'feature' => array(
              'mark' => array(
                'show' => TRUE,
                'title' => array(
                  'mark' => t('Marcar'),
                  'markUndo' => t('Deshacer'),
                  'markClear' => t('Limpiar Marcas')
                )
              ),
              'dataView' => array(
              'show' => TRUE,
              'readOnly' => false,
              'title' => t('Ver Datos'),
              'lang' => array(
                t('Datos'),
                t('Regresar'),
                ''
              )          
            ),
            'restore' => array(
              'show' => TRUE,
              'title' => t('Refrescar')
            ),
            'saveAsImage' => array(
              'show' => TRUE,
              'title' => t('Guardar'),
              'type' => 'png'
            ),
          )
        ),
        'calculable' => true,
        'polar' => array(
            array(
                'indicator' => echarts_field_create_polar_radar($xaxis,$json,$series_x,$max_y),
                'radius' => '80%'
              )
        ),
        'series' => array(
          array(
              'name' => $title,
              'type' => 'radar',
              'itemStyle' => array(
                'normal'=> array(
                  'areaStyle' => array(
                    'type'=> 'default'
                    )
                  )
              ),
              'data' => echarts_field_get_series($xaxis, $series, $json, $type, $series_x)
          )
        ) 
      )
    );
    return $radar; 
}


/**
*Created by Randall Sibaja R. 
*
**/
function echarts_field_generate_settings_treemap_graph($json,$xaxis , $series, $type, $series_x, $title='')
{
  $treemap = array(
    'chartType' => $type,
    'useType' => array($type),
    'containerId' => 'tree_chart',
    'options' => array(
      'tooltip' => array(
        'trigger' => 'item',
        'formatter' => '{b}: {c}'
      ),      
      'title' => array(
        'text' => $title,
        'subtext' => $series_x,
        'x'=> 'center'
      ),
      'toolbox' => array(
        'show' => TRUE,
        'feature' => array(
          'mark' => array(
            'show' => TRUE,
            'title' => array(
              'mark' => t('Marcar'),
              'markUndo' => t('Deshacer'),
              'markClear' => t('Limpiar Marcas')
              )
          ),
          'dataView' => array(
            'show' => TRUE,
            'readOnly' => false,
            'title' => t('Ver Datos'),
            'lang' => array(t('Datos'), t('Regresar'), '')          
          ),
          'restore' => array(
            'show' => TRUE,
            'title' => t('Refrescar')
          ),
          'saveAsImage' => array(
            'show' => TRUE,
            'title' => t('Guardar'),
            'type' => 'png'
          )
        )
      ),
      'calculable' => false,
      'series' => array(
        array(
          'name'=> t($series_x),
          'type'=> $type,
          'itemStyle' => array(
            'normal' => array(
              'label' => array(
                'show'=>true,
                'formatter' => '{b}'
              ),
              'borderWidth' => 1
            ),
            'emphasis' => array(
              'label' => array(
                'show' => true
              )
            )
          ),
          'data'=> echarts_field_get_series($xaxis, $series, $json, $type, $series_x)
        )          
      )
    )
  );
  return $treemap;
}



