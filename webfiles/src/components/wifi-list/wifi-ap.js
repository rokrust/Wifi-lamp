import React, {Component} from 'react'
import styled from 'styled-components'
import WifiIcon from './wifi-icon'

export default class WifiAp extends Component {
    constructor(props) {
        super(props);
        this.state = {
            name: "",
            signalStrength: 0
        }
    }

    componentDidMount() {
        this.setState({
            name: this.props.name,
            signalStrength: this.props.signalStrength
        });
    }
    
    render() {
        return (
            <div>
                <ApWrap>
                    <WifiName>{this.state.name}</WifiName>
                    <IconWrapper>
                        <WifiIcon signalStrength={this.state.signalStrength} width="5em" height="5em"/>
                    </IconWrapper>
                </ApWrap>
            </div>
        );
    }
}

/*<form style={{display: "flex", flexDirection: "row"}}>
    <input/><br/>
    <input type="submit" />
</form>*/

const ApWrap = styled.div`
    display: flex;
    flex-direction: row;
    justify-content: space-between;
    padding: 0em 10em;
    border: groove;
`;

const WifiName = styled.p`
    align-self: left;
    font-size: 1.7em;
    position: relative;
    left: 3em;
`;

const IconWrapper = styled.div`
    align-self: right;
    position: relative;
    right: 3em;
`;