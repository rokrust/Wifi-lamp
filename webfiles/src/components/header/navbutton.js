import React, {Component} from 'react'
import styled from 'styled-components'
import { Link } from 'react-router-dom'


/*const NavButton = () => {
    useState();
}*/

export default class NavButton extends Component {
    render() {
        return (
            <Link to={this.props.to} style={{ textDecoration: 'none' }}>
                <RouterButton onMouseLeave={() => this.setState}>
                    <ButtonText>
                        {this.props.children}
                    </ButtonText>
                </RouterButton>
            </Link>
        )

    }
}



const RouterButton = styled.div`
    height: 2em;
    width: 3em;
    padding: 0.5em 1em;
    background-color: ${props => props.theme.primaryLight};
    &:hover {
        & > span {
            border-bottom: 0.1em solid ${props => props.theme.primaryText};
        }
    }
`;

const ButtonText = styled.span`
    color: ${props => props.theme.primaryText};
`;