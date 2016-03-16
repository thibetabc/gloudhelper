<GameProjectFile>
  <PropertyGroup Type="Layer" Name="findout_layer" ID="4909c01f-698e-4334-8c54-d84a0ab8e1a5" Version="2.0.6.0" />
  <Content ctype="GameProjectContent">
    <Content>
      <Animation Duration="0" Speed="1.0000" />
      <ObjectData Name="Layer" FrameEvent="" Tag="42" ctype="LayerObjectData">
        <Position X="0.0000" Y="0.0000" />
        <Scale ScaleX="1.0000" ScaleY="1.0000" />
        <AnchorPoint />
        <CColor A="255" R="255" G="255" B="255" />
        <Size X="1280.0000" Y="720.0000" />
        <PrePosition X="0.0000" Y="0.0000" />
        <PreSize X="0.0000" Y="0.0000" />
        <Children>
          <NodeObjectData Name="bg_layout" ActionTag="-1726824901" Rotation="-90.0000" RotationSkewX="-90.0000" RotationSkewY="-90.0000" FrameEvent="" Tag="24" ObjectIndex="1" TouchEnable="True" BackColorAlpha="127" ComboBoxIndex="1" ColorAngle="90.0000" ctype="PanelObjectData">
            <Position X="1280.0000" Y="0.0000" />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <AnchorPoint />
            <CColor A="255" R="255" G="255" B="255" />
            <Size X="720.0000" Y="1280.0000" />
            <PrePosition X="1.0000" Y="0.0000" />
            <PreSize X="0.5625" Y="1.7778" />
            <Children>
              <NodeObjectData Name="findout_layout" ActionTag="907811582" FrameEvent="" Tag="44" ObjectIndex="1" Scale9Enable="True" LeftEage="7" RightEage="7" TopEage="7" BottomEage="7" Scale9OriginX="7" Scale9OriginY="7" Scale9Width="8" Scale9Height="8" ctype="ImageViewObjectData">
                <Position X="360.0000" Y="640.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <CColor A="255" R="255" G="255" B="255" />
                <Size X="600.0000" Y="420.0000" />
                <PrePosition X="0.5000" Y="0.5000" />
                <PreSize X="0.8333" Y="0.3281" />
                <Children>
                  <NodeObjectData Name="line_sprite" ActionTag="-1851509726" FrameEvent="" Tag="45" ObjectIndex="2" ctype="SpriteObjectData">
                    <Position X="300.0000" Y="324.0000" />
                    <Scale ScaleX="1.0750" ScaleY="1.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <Size X="558.0000" Y="6.0000" />
                    <PrePosition X="0.5000" Y="0.7714" />
                    <PreSize X="0.0000" Y="0.0000" />
                    <FileData Type="Normal" Path="img/green_line.png" />
                  </NodeObjectData>
                  <NodeObjectData Name="title_text" ActionTag="-1347869024" FrameEvent="" Tag="46" ObjectIndex="1" FontSize="36" LabelText="发现新版本" ctype="TextObjectData">
                    <Position X="300.0000" Y="370.6700" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <CColor A="255" R="0" G="0" B="0" />
                    <Size X="180.0000" Y="36.0000" />
                    <PrePosition X="0.5000" Y="0.8825" />
                    <PreSize X="0.3000" Y="0.0857" />
                  </NodeObjectData>
                  <NodeObjectData Name="info_text" ActionTag="-907817297" FrameEvent="" Tag="47" ObjectIndex="2" FontSize="36" LabelText="发现有新版本,是否升级?" IsCustomSize="True" HorizontalAlignmentType="HT_Center" ctype="TextObjectData">
                    <Position X="300.0000" Y="200.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <CColor A="255" R="0" G="0" B="0" />
                    <Size X="600.0000" Y="180.0000" />
                    <PrePosition X="0.5000" Y="0.3226" />
                    <PreSize X="1.0000" Y="0.4286" />
                  </NodeObjectData>
                  <NodeObjectData Name="skip_button" ActionTag="674434334" FrameEvent="" Tag="48" ObjectIndex="1" TouchEnable="True" FontSize="36" ButtonText="取消" Scale9Width="278" Scale9Height="83" ctype="ButtonObjectData">
                    <Position X="150.0000" Y="41.0000" />
                    <Scale ScaleX="1.0800" ScaleY="1.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <Size X="278.0000" Y="83.0000" />
                    <PrePosition X="0.2500" Y="0.0976" />
                    <PreSize X="0.0000" Y="0.0000" />
                    <TextColor A="255" R="0" G="0" B="0" />
                    <DisabledFileData Type="Normal" Path="img/left_white_button.png" />
                    <PressedFileData Type="Normal" Path="img/left_green_button.png" />
                    <NormalFileData Type="Normal" Path="img/left_white_button.png" />
                  </NodeObjectData>
                  <NodeObjectData Name="retry_button" ActionTag="-2008696933" FrameEvent="" Tag="51" ObjectIndex="3" TouchEnable="True" FontSize="36" ButtonText="确定" Scale9Width="278" Scale9Height="83" ctype="ButtonObjectData">
                    <Position X="450.0000" Y="41.0000" />
                    <Scale ScaleX="1.0800" ScaleY="1.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <Size X="278.0000" Y="83.0000" />
                    <PrePosition X="0.7500" Y="0.0976" />
                    <PreSize X="0.0000" Y="0.0000" />
                    <TextColor A="255" R="0" G="0" B="0" />
                    <DisabledFileData Type="Normal" Path="img/right_white_button.png" />
                    <PressedFileData Type="Normal" Path="img/right_green_button.png" />
                    <NormalFileData Type="Normal" Path="img/right_white_button.png" />
                  </NodeObjectData>
                </Children>
                <FileData Type="Normal" Path="img/white_bg.9.png" />
              </NodeObjectData>
            </Children>
            <SingleColor A="255" R="0" G="0" B="0" />
            <FirstColor A="255" R="0" G="0" B="0" />
            <EndColor A="255" R="255" G="255" B="255" />
            <ColorVector ScaleY="1.0000" />
          </NodeObjectData>
        </Children>
      </ObjectData>
    </Content>
  </Content>
</GameProjectFile>